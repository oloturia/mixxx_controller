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

//digital switches
const int double_interval = 100;

struct digitalButton  {
  const int pin;
  bool pressed;
  int debounce;
  int pres_state;
  int countdown;
  const byte effect;
  const byte effect_double;
};

digitalButton sw0 = {9,false,0,double_interval,LOW,14,21};
digitalButton sw1 = {11,false,0,double_interval,LOW,15,22};
digitalButton sw2 = {8,false,0,double_interval,LOW,16,23};
digitalButton sw3 = {7,false,0,double_interval,LOW,17,24};
digitalButton sw4 = {13,false,0,double_interval,LOW,20,25};

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
  for (int i = 0; i < active_analog_controls; i++) {
    analog_controls[i].value = analogRead(analog_controls[i].pin);
    
    if ( abs(analog_controls[i].value - analog_controls[i].prev_value) > 9 ){

      midi_value = map(analog_controls[i].value,0,1023,0,127);
      controlChange(1,analog_controls[i].effect,midi_value);
      #ifdef DEBUG
        Serial.print("Sent ");
        Serial.print(analog_controls[i].effect);
        Serial.print(",");      
        Serial.println(analog_controls[i].value);
        Serial.print(",");
        Serial.println( abs(analog_controls[i].value - analog_controls[i].prev_value) );
      #endif
      analog_controls[i].prev_value = analog_controls[i].value;
      
      MidiUSB.flush();
    }
  }
  

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
}
