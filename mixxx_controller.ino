#include "Arduino.h"
#include "MIDIUSB.h"
#include <EncoderButton.h>
#define DEBUG

//analog controls (sliders and potentiometers), the deadzone is the smallest increment that the control must have to trigger the effect
const int dead_zone = 9;

struct analogControl {
  const int pin;
  int value;
  int prev_value;
  const byte effect;
};

analogControl slider_0 = {A0,0,0,7};
analogControl slider_1 = {A1,0,0,8};
analogControl slider_2 = {A2,0,0,9};

analogControl knob_0 = {A3,0,0,1};
analogControl knob_1 = {A4,0,0,2};
analogControl knob_2 = {A5,0,0,3};

analogControl knob_3 = {A7,0,0,4};
analogControl knob_4 = {A10,0,0,5};
analogControl knob_5 = {A11,0,0,6};


analogControl analog_controls[] = {slider_0,slider_1,slider_2,knob_0,knob_1,knob_2,knob_3,knob_4,knob_5};
const int active_analog_controls = sizeof(analog_controls)/sizeof(analog_controls[0]);

//digital encoders, the fd is forward, bk backward effects, clk should be triggers pins
struct rotaryEncoder {
  const int clk;
  const int dt;
  volatile int val;
  volatile int val_changed;
  EncoderButton eb;
  const int effect_fd;
  const int effect_bk;
};

rotaryEncoder rE_0 = {2,4,0,false,EncoderButton (2,4),10,11};
rotaryEncoder rE_1 = {3,5,0,false,EncoderButton (3,5),12,13};
rotaryEncoder rE_2 = {0,1,0,false,EncoderButton (0,1),18,19};

rotaryEncoder rotary_encoders[] = {rE_0,rE_1,rE_2};
const int active_encoders = sizeof(rotary_encoders)/sizeof(rotary_encoders[1]);

//digital switches, double_interval is the interval of double clicks, if effect_double is 0 the button is immediate, toggle send two different messages for odd and even strokes
const int double_interval = 400;

struct digitalButton  {
  const int pin;
  bool pressed;
  long debounce;
  int pres_state;
  long countdown;
  const byte effect;
  const byte effect_toggle;
  bool toggled;
  const byte effect_double;
};

digitalButton sw0 = {9,false,0,0,LOW,14,0,false,0};
digitalButton sw1 = {11,false,0,0,LOW,15,0,false,0};
digitalButton sw2 = {8,false,0,0,LOW,16,0,false,23};
digitalButton sw3 = {7,false,0,0,LOW,17,0,false,24};
digitalButton sw4 = {13,false,0,0,LOW,25,26,false,20};

digitalButton switches[] = {sw0,sw1,sw2,sw3,sw4};
const int active_switches = sizeof(switches)/sizeof(switches[0]);

//midi
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
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
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

  //analog controls
  for (int i = 0; i < active_analog_controls; i++) {
    analog_controls[i].value = analogRead(analog_controls[i].pin);    
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
  
  //rotary encoders
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

  //switches
  for (int i = 0; i < active_switches; i++) {
    if (digitalRead(switches[i].pin) == switches[i].pres_state) {
      switches[i].pressed = true;
      switches[i].debounce = millis();
    }
    if (switches[i].pressed && (millis() - switches[i].debounce > 10) && digitalRead(switches[i].pin) != switches[i].pres_state) {
      if (switches[i].effect_double == 0) {
        if (switches[i].effect_toggle == 0) {
          controlChange(1,switches[i].effect,1);
        } else if (switches[i].toggled) {
          controlChange(1,switches[i].effect,1);
          switches[i].toggled = false;
        } else {
          controlChange(1,switches[i].effect_toggle,1);
          switches[i].toggled = true;
        }
        #ifdef DEBUG
         Serial.print("Click SW");
         Serial.println(switches[i].pin);
        #endif
        MidiUSB.flush();        
      } else if (switches[i].countdown > 0) {
        switches[i].countdown = 0;
        controlChange(1,switches[i].effect_double,1);
        #ifdef DEBUG
          Serial.print("Double click SW");
          Serial.println(switches[i].pin);
        #endif
        MidiUSB.flush();
      } else {
        switches[i].countdown = millis();
      }
      switches[i].pressed = false;
    }
    if (switches[i].countdown > 0 and millis() - switches[i].countdown > double_interval) {
      switches[i].countdown = 0;
      if (switches[i].effect_toggle == 0) {
        controlChange(1,switches[i].effect,1);
      } else if (switches[i].toggled) {
        controlChange(1,switches[i].effect,1);
        switches[i].toggled = false;
      } else {
        controlChange(1,switches[i].effect_toggle,1);
        switches[i].toggled = true;
      }
      
      #ifdef DEBUG
         Serial.print("Single click SW");
         Serial.println(switches[i].pin);
      #endif
      MidiUSB.flush();
    }

  }
}
