#include "MIDIUSB.h"
#include <light_CD74HC4067.h>

CD74HC4067 mux(4, 5, 6, 7); 
const int signal_pin = A0; 

struct Control {
  const int chan;
  int value;
  int prev_value;
  const byte effect;
};

Control crossfader = {6,0,0,0};

Control h_tones_l = {0,0,0,1};
Control m_tones_l = {1,0,0,2};
Control l_tones_l = {2,0,0,3};

Control h_tones_r = {3,0,0,4};
Control m_tones_r = {4,0,0,5};
Control l_tones_r = {5,0,0,6};


Control controls[] = {crossfader,h_tones_l,m_tones_l,l_tones_l,h_tones_r,m_tones_r,l_tones_r};
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
  while(!Serial);
  Serial.begin(115200);
  Serial.println("RDY");
  pinMode(signal_pin, INPUT);
}

void loop() {
  for (int i = 0; i < active_controls; i++) {
    mux.channel(controls[i].chan);
    
    controls[i].value = analogRead(signal_pin);
    if ( abs(controls[i].value - controls[i].prev_value) > 9 ){

      midi_value = map(controls[i].value,0,1023,0,127);
      controlChange(1,controls[i].effect,midi_value);
      
//      Serial.print("Sent ");
//      Serial.print(controls[i].effect);
//      Serial.print(",");      
//      Serial.print(controls[i].value);
//      Serial.print(",");
//      Serial.print(controls[i].prev_value);
//      Serial.print(",");
//      Serial.println( abs(controls[i].value - controls[i].prev_value) );
      controls[i].prev_value = controls[i].value;
      
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
