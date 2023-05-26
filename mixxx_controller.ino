#include "MIDIUSB.h"

struct Control {
  const int pin;
  unsigned int value;
  unsigned int prev_value;
  const byte effect;
  const bool no_side;
};

Control crossfader = {A0,0,0,1,true};
Control bpm = {A1,0,0,2,false};
Control h_tones = {A2,0,0,3,false};
Control m_tones = {A3,0,0,4,false};
Control l_tones = {A4,0,0,5,false};
Control controls[] = {crossfader,bpm,h_tones,m_tones,l_tones};
const int active_controls = sizeof(controls)/sizeof(controls[0]);

byte midi_value;

const int side = 7;

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
  while(!Serial);
  Serial.begin(115200);
  Serial.println("RDY");
  pinMode(crossfader.pin,INPUT);
  pinMode(bpm.pin,INPUT);
  pinMode(h_tones.pin,INPUT);
  pinMode(m_tones.pin,INPUT);
  pinMode(l_tones.pin,INPUT);
  pinMode(side,INPUT);
}

void loop() {
  for (int i = 0; i < active_controls; i++) {
    controls[i].value = analogRead(controls[i].pin);
    if ( abs(controls[i].value - controls[i].prev_value) > 3 ){
      controls[i].prev_value = controls[i].value;
      midi_value = map(controls[i].value,0,1023,0,127);
      if ( digitalRead(side) == HIGH || controls[i].no_side) {
        controlChange(1,controls[i].effect,controls[i].value);
      } else {
        controlChange(2,controls[i].effect,controls[i].value);
      }
      Serial.print("Sent ");
      Serial.print(controls[i].effect);
      if ( digitalRead(side) == HIGH || controls[i].no_side) {
        Serial.print(",R,");
      } else {
        Serial.print(",L,");
      }
      Serial.print(controls[i].value);
      Serial.print(",");
      Serial.print(controls[i].prev_value);
      Serial.print(",");
      Serial.println( abs(controls[i].value - controls[i].prev_value) );
      delay(10);
      MidiUSB.flush();
    }
  }
  /*crossfader.value = analogRead(crossfader.pin);
  bpm.value = analogRead(bpm.pin);
  h_tones.value = analogRead(h_tones.pin);
  m_tones.value = analogRead(m_tones.pin);
  l_tones.value = analogRead(l_tones.pin);
  if ( abs(value-prev_value) > 3) {
    prev_value = value;
    midi_value = map(value,0,1023,0,127);
    controlChange(1,10,midi_value);
    delay(10);
    MidiUSB.flush();
  } */

}
