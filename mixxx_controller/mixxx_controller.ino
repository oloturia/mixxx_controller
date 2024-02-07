#include "Arduino.h"
#include "MIDIUSB.h"
#include <EncoderButton.h>

/* This is a sketch that it transforms your Arduino Leonardo or Leonardo like (i.e. boards with atmega32u4) in a MIDI console for interfacing
 * with Mixxx (https://mixxx.org). Perhaps you can also use an UNO but you have to sacrifice some pins in order to add a MIDI interface to your Arduino.
 * Once you have flashed the board, you have to train Mixxx in Preferences/Controllers menu.
 * Remember to plug the Arduino before launching Mixxx, otherwise it won't be detected.
 * 
 * This software is released under the Unlicense (see LICENSE for more info).
 * 
 * Last revision 27-dec-2023
 */

// Uncomment this line if you want pin and values printed on the serial, remember to open a console because it waits until a connection is established.
// #define DEBUG

// Analog controls (sliders and potentiometers), the deadzone is the smallest increment that the control must have to trigger the effect.
const int dead_zone = 15;

// Stickyness is a deazone that lies around extreme positions (0-1023) and the middle (511, but trimmable) of readings.
const int stickyness = 30;

struct analogControl {
  const int pin;
  int value;
  int prev_value;
  const byte effect;
  const int middle;
};

/* Connect linear potentiometers (sliders and knobs) to Analog Inputs on your board.
 * On Leonardo, from A0 to A5 are on the left (with USB on top)
 * A6, A7, A8, A9, A10 and A11 are digital pins:
 *  4,  6,  8,  9,  10,     12.
 *  
 * Change the first value accordingly, the last one is the MIDI effect that is sent to
 * Mixxx, every control needs to have a unique number.
*/

//If you aren't using any analog controls, comment these lines.
const int analog_controls_pins[] = {A0,A1,A2,A3,A4,A5,A7,A10,A11};
const int analog_controls_effects[] = {7,8,9,1,2,3,4,5,6};

analogControl analog_controls[] = {
  analogControl {analog_controls_pins[0],0,0,analog_controls_effects[0],511},
  analogControl {analog_controls_pins[1],0,0,analog_controls_effects[1],520},
  analogControl {analog_controls_pins[2],0,0,analog_controls_effects[2],520},
  
  analogControl {analog_controls_pins[3],0,0,analog_controls_effects[3],511},
  analogControl {analog_controls_pins[4],0,0,analog_controls_effects[4],511},
  analogControl {analog_controls_pins[5],0,0,analog_controls_effects[5],511},
  
  analogControl {analog_controls_pins[6],0,0,analog_controls_effects[6],511},
  analogControl {analog_controls_pins[7],0,0,analog_controls_effects[7],511},
  analogControl {analog_controls_pins[8],0,0,analog_controls_effects[8],511},
};
const int active_analog_controls = sizeof(analog_controls)/sizeof(analog_controls[0]);

//These lines disable analog controls if uncommented.
//analogControl analog_controls[] = {}; 
//const int active_analog_controls = 0;

/* Rotary Encorders need two pins. 
 * clk should be an interrupt. On Leonardo interrupts are on pin 0, 1, 2, 3 and 7.
 * dt are digital pins
 * effect_fd and effect_bk have to be unique
 */ 
struct rotaryEncoder {
  const int clk;
  const int dt;
  volatile int val;
  volatile int val_changed;
  EncoderButton eb;
  const int effect_fd;
  const int effect_bk;
};

//Comment these lines if you don't have any rotary encoders.
const int rot_clk_pins[] = {2,3,0};
const int rot_dt_pins[] = {4,5,1};
const int rot_effect_fd[] = {10,12,18};
const int rot_effect_bk[] = {11,13,19};

rotaryEncoder rotary_encoders[] = {
 {rot_clk_pins[0],rot_dt_pins[0],0,false,EncoderButton (rot_clk_pins[0],rot_dt_pins[0]),rot_effect_fd[0],rot_effect_bk[0]},
 {rot_clk_pins[1],rot_dt_pins[1],0,false,EncoderButton (rot_clk_pins[1],rot_dt_pins[1]),rot_effect_fd[1],rot_effect_bk[1]},
 {rot_clk_pins[2],rot_dt_pins[2],0,false,EncoderButton (rot_clk_pins[2],rot_dt_pins[2]),rot_effect_fd[2],rot_effect_bk[2]},
};
const int active_encoders = sizeof(rotary_encoders)/sizeof(rotary_encoders[1]);

//These lines disable rotary encoders if uncommented.
//rotaryEncoder rotary_encoders[] = {};
//const int active_encoders = 0;

/* Digital buttons are connected to a digital pin. If you use toggle, long press or long press toggle rembember that the MIDI message byte has to be unique.
 * long_inteval is the interval of long clicks, toggle and long_toggle (for long presses) send two different messages for odd and even strokes.
 * If effect is 0 the toggle, long press or long press toggle is disabled. 
 */
const int long_interval = 1000;

struct digitalButton  {
  const int pin;
  bool pressed;
  long debounce;
  long countdown;
  const byte effect;
  const byte effect_toggle;
  bool toggled;
  const byte effect_long;
  const byte effect_long_toggle;
  bool long_toggled;
  bool long_pressed;
};

//Comment these lines if you don't use switches.
const int button_pins[] = {9,11,8,7,13};
const int button_effect[] = {14,15,16,17,20};
const int button_effect_toggle[] = {0,0,0,0,0};
const int button_effect_long[] {0,23,0,24,25};
const int button_effect_toggle_long[] {0,0,0,26,0};
digitalButton switches[] = {
  {button_pins[0],false,0,0,button_effect[0],button_effect_toggle[0],false,button_effect_long[0],button_effect_toggle_long[0],false,false},
  {button_pins[1],false,0,0,button_effect[1],button_effect_toggle[1],false,button_effect_long[1],button_effect_toggle_long[1],false,false},
  {button_pins[2],false,0,0,button_effect[2],button_effect_toggle[2],false,button_effect_long[2],button_effect_toggle_long[2],false,false},
  {button_pins[3],false,0,0,button_effect[3],button_effect_toggle[3],false,button_effect_long[3],button_effect_toggle_long[3],false,false},
  {button_pins[4],false,0,0,button_effect[4],button_effect_toggle[4],false,button_effect_long[4],button_effect_toggle_long[4],false,false}
};
const int active_switches = sizeof(switches)/sizeof(switches[0]);

//These lines disables digital buttons.
//digitalButton switches[] = {};
//const int active_switches = 0;

// MIDI
byte midi_value;

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  #ifdef DEBUG
    Serial.print("Note on:");
    Serial.print(channel);
    Serial.print(",");
    Serial.print(pitch);
    Serial.print(",");
    Serial.println(velocity);
  #endif
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
   #ifdef DEBUG
    Serial.print("Note off:");
    Serial.print(channel);
    Serial.print(",");
    Serial.print(pitch);
    Serial.print(",");
    Serial.println(velocity);
  #endif
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  #ifdef DEBUG
    Serial.print("Control change:");
    Serial.print(channel);
    Serial.print(",");
    Serial.print(control);
    Serial.print(",");
    Serial.println(value);
  #endif
  MidiUSB.sendMIDI(event);
}

int eb_Encoder(EncoderButton& eb) {
  for (int i = 0; i < active_encoders; i++) {
    if (&eb == &rotary_encoders[i].eb){
      rotary_encoders[i].val = eb.increment();
      rotary_encoders[i].val_changed = true;
    }
  }
}

void setup() {
  #ifdef DEBUG
    while(!Serial);
    Serial.begin(115200);
    Serial.println("RDY");
  #endif

  for (int i = 0; i < active_switches; i++) {
    pinMode(switches[i].pin,INPUT_PULLUP);
  }

  for (int i = 0; i < active_encoders; i++) {
    rotary_encoders[i].eb.setEncoderHandler(eb_Encoder);
  }
}

void loop() {
  // Analog controls check.
  for (int i = 0; i < active_analog_controls; i++) {
    analog_controls[i].value = analogRead(analog_controls[i].pin);
    if (analog_controls[i].value < 0+stickyness){
        analog_controls[i].value = 0;
    } else if (analog_controls[i].value > 1023-stickyness) {
        analog_controls[i].value = 1023;
    } else if (analog_controls[i].value > analog_controls[i].middle-stickyness && analog_controls[i].value < analog_controls[i].middle+stickyness) {
        analog_controls[i].value = analog_controls[i].middle;
    }
    if ( abs(analog_controls[i].value - analog_controls[i].prev_value) > dead_zone ){
      midi_value = map(analog_controls[i].value,0,1023,0,127);
      controlChange(1,analog_controls[i].effect,midi_value);
      #ifdef DEBUG
        Serial.print("Sent ");
        Serial.print(analog_controls[i].effect);
        Serial.print(",");      
        Serial.print(analog_controls[i].value);
        Serial.print(",");
        Serial.println( abs(analog_controls[i].value - analog_controls[i].prev_value) );
      #endif
      analog_controls[i].prev_value = analog_controls[i].value;
      MidiUSB.flush();
    }
  }
  
  // Rotary encoders check.
  for (int i = 0; i < active_encoders; i++) {
    rotary_encoders[i].eb.update();  
    if(rotary_encoders[i].val_changed) {
      if(rotary_encoders[i].val < 0){
        controlChange(1,rotary_encoders[i].effect_fd,1);
      } else {
        controlChange(1,rotary_encoders[i].effect_bk,1);
      }
      rotary_encoders[i].val_changed = false;
      MidiUSB.flush();
      #ifdef DEBUG
        Serial.print(i);
        Serial.print(" enc=");
        Serial.println(rotary_encoders[i].val);
      #endif
    }
  }

  // Switches check.
  for (int i = 0; i < active_switches; i++) {
    if (digitalRead(switches[i].pin) == LOW && switches[i].pressed == false) {
      switches[i].pressed = true;
      switches[i].long_pressed = false;
      switches[i].debounce = millis();
      #ifdef DEBUG
        Serial.print("Press SW ");
        Serial.println(switches[i].pin);
      #endif DEBUG
    }
    if (switches[i].pressed && (millis() - switches[i].debounce <= 10) && digitalRead(switches[i].pin) == HIGH) {
      switches[i].pressed = false;
      continue;
    }
    if (switches[i].pressed && (millis() - switches[i].debounce > 10) && digitalRead(switches[i].pin) == HIGH) {
        if (switches[i].long_pressed == true) {
          switches[i].long_pressed = false;
        } else if (switches[i].effect_toggle == 0) {
          controlChange(1,switches[i].effect,1);
          #ifdef DEBUG
            Serial.print("Release SW ");
            Serial.println(switches[i].pin);
          #endif
        } else if (switches[i].toggled) {
          controlChange(1,switches[i].effect,1);
          switches[i].toggled = false;
          #ifdef DEBUG
            Serial.print("Release SW - toggle 0 ");
            Serial.println(switches[i].pin);
          #endif
        } else {
          controlChange(1,switches[i].effect_toggle,1);
          switches[i].toggled = true;
          #ifdef DEBUG
            Serial.print("Release SW - toggle 1 ");
            Serial.println(switches[i].pin);
          #endif
        }

      MidiUSB.flush();        
      switches[i].pressed = false;
    }
    if (switches[i].pressed && (millis() - switches[i].debounce > long_interval) && (digitalRead(switches[i].pin) == LOW) && (switches[i].long_pressed == false)) {
      switches[i].long_pressed = true;
      if (switches[i].effect_long_toggle == 0) {
        controlChange(1,switches[i].effect_long,1);
        #ifdef DEBUG
          Serial.print("Long press SW ");
          Serial.println(switches[i].pin);
        #endif
      } else if (switches[i].long_toggled) {
        controlChange(1,switches[i].effect_long,1);
        switches[i].long_toggled = false;
        #ifdef DEBUG
          Serial.print("Long press SW - toggle 0 ");
          Serial.println(switches[i].pin);
        #endif        
      } else {
        controlChange(1,switches[i].effect_long_toggle,1);
        switches[i].long_toggled = true;
        #ifdef DEBUG
          Serial.print("Long press SW - toggle 1 ");
          Serial.println(switches[i].pin);
        #endif                
      }
      MidiUSB.flush();
      
    }
  }
}
