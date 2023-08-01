#include "Arduino.h"
#include "MIDIUSB.h"
#include <EncoderButton.h>
#define DEBUG

//analog controls (sliders and potentiometers)
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

//digital encoders
const int clk_0 = 2;
const int clk_1 = 3;

const int dt_0 = 4;
const int dt_1 = 5;

volatile int val_0 = 0;
volatile int val_1 = 0;
volatile int val_0_changed = false;
volatile int val_1_changed = false;

EncoderButton eb_0(clk_0,dt_0);
EncoderButton eb_1(clk_1,dt_1);

//digital switches
struct digitalButton  {
  const int pin;
  bool pressed;
  int debounce;
  int pres_state;
  const byte effect;
};

digitalButton sw0 = {9,false,0,LOW,14};
digitalButton sw1 = {11,false,0,LOW,15};
digitalButton sw2 = {8,false,0,LOW,16};
digitalButton sw3 = {7,false,0,LOW,17};

digitalButton switches[] = {sw0,sw1,sw2,sw3};
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

void eb_Encoder(EncoderButton& eb) {
  if(&eb == &eb_0) {
    val_0 = eb.increment();
    val_0_changed = true;
  } else {
    val_1 = eb.increment();
    val_1_changed = true;
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
  
  eb_0.setEncoderHandler(eb_Encoder);
  eb_1.setEncoderHandler(eb_Encoder);
  
}

.
void loop() {
  for (int i = 0; i < active_analog_controls; i++) {
    analog_controls[i].value = analogRead(analog_controls[i].pin);
    
    if ( abs(analog_controls[i].value - analog_controls[i].prev_value) > 9 ){

      midi_value = map(analog_controls[i].value,0,1023,0,127);
      controlChange(1,analog_controls[i].effect,midi_value);
      #ifdef DEBUG
        Serial.print("Sent ");
        Serial.print(analog_controls[i].effect);
        Serial.print(",");      
        Serial.print(analog_controls[i].value);
        Serial.print(",");
        Serial.print(analog_controls[i].prev_value);
        Serial.print(",");
        Serial.println( abs(analog_controls[i].value - analog_controls[i].prev_value) );
      #endif
      analog_controls[i].prev_value = analog_controls[i].value;
      
      MidiUSB.flush();
    }
  }
  
  eb_0.update();
  eb_1.update();
  
  if (val_0_changed) {
    if (val_0 < 0) {
      controlChange(1,10,1);
    } else {
      controlChange(1,11,1);
    }
      #ifdef DEBUG
      Serial.print("val_0=");Serial.println(val_0);
      #endif
    val_0_changed = false;
    MidiUSB.flush();
  }
  if (val_1_changed) {
    if (val_1 < 0) {
      controlChange(1,12,1);
    } else {
      controlChange(1,13,1);
    }
      #ifdef DEBUG
      Serial.print("val_1=");Serial.println(val_1);
      #endif
    val_1_changed = false;
    MidiUSB.flush();
  }  
  for (int i = 0; i < active_switches; i++) {
    if (digitalRead(switches[i].pin) == switches[i].pres_state) {
      switches[i].pressed = true;
      switches[i].debounce = millis();
    }
    if (switches[i].pressed && (millis() - switches[i].debounce > 10) && digitalRead(switches[i].pin) != switches[i].pres_state) {
      controlChange(1,switches[i].effect,1);
      switches[i].pressed = false;
      #ifdef DEBUG
      Serial.print("Pressed SW");
      Serial.println(switches[i].pin);
      #endif
      MidiUSB.flush();
    }
  }

  /*
  if (digitalRead(sw_0) == LOW) {
    sw_0_pressed = true;
    sw_0_debounce = millis();
  }
  if (digitalRead(sw_1) == LOW) {
    sw_1_pressed = true;
    sw_1_debounce = millis();
  }
  if (sw_0_pressed && (millis() - sw_0_debounce > 10) && digitalRead(sw_0) == HIGH) {
    controlChange(1,14,1);
    sw_0_pressed = false;
      #ifdef DEBUG
      Serial.println("sw0 pressed");
      #endif

    MidiUSB.flush();
  }
  if (sw_1_pressed && (millis() - sw_1_debounce > 10) && digitalRead(sw_1) == HIGH) {
    controlChange(1,15,1);
    sw_1_pressed = false;
      #ifdef DEBUG
      Serial.println("sw1 pressed");
      #endif
    MidiUSB.flush();    
  }
*/
}
