#include "Arduino.h"
#include "MIDIUSB.h"
//#define DEBUG

struct Control {
  const int pin;
  int value;
  int prev_value;
  const byte effect;
};

Control slider_0 = {A0,0,0,7};
Control slider_1 = {A1,0,0,8};
Control slider_2 = {A2,0,0,9};

Control knob_0 = {A3,0,0,1};
Control knob_1 = {A4,0,0,2};
Control knob_2 = {A5,0,0,3};

Control knob_3 = {A7,0,0,4};
Control knob_4 = {A10,0,0,5};
Control knob_5 = {A11,0,0,6};


Control controls[] = {slider_0,slider_1,slider_2,knob_0,knob_1,knob_2,knob_3,knob_4,knob_5};
const int active_controls = sizeof(controls)/sizeof(controls[0]);

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

void setup() {
  #ifdef DEBUG
    while(!Serial);
    Serial.begin(115200);
    Serial.println("RDY");
  #endif
}

void loop() {
  for (int i = 0; i < active_controls; i++) {
    controls[i].value = analogRead(controls[i].pin);
    
    if ( abs(controls[i].value - controls[i].prev_value) > 9 ){

      midi_value = map(controls[i].value,0,1023,0,127);
      controlChange(1,controls[i].effect,midi_value);
      #ifdef DEBUG
        Serial.print("Sent ");
        Serial.print(controls[i].effect);
        Serial.print(",");      
        Serial.print(controls[i].value);
        Serial.print(",");
        Serial.print(controls[i].prev_value);
        Serial.print(",");
        Serial.println( abs(controls[i].value - controls[i].prev_value) );
      #endif
      controls[i].prev_value = controls[i].value;
      
      delay(10);
      MidiUSB.flush();
    }
  }
}
