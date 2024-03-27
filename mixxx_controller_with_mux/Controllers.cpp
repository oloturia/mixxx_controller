#include "Controllers.h"

// MIDI

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

// Constructor and methods for analog controls and buttons (encoders use a different library)

AnalogControl::AnalogControl(int pin, byte effect, int middle) {
  this->pin = pin;
  this->effect = effect;
  this->middle = middle;
  pinMode(pin,INPUT);
}

void AnalogControl::checkStatus() {
  value = analogRead(pin);
  if (value < 0+STICKYNESS){
      value = 0;
  } else if (value > 1023-STICKYNESS) {
      value = 1023;
  } else if (value > middle-STICKYNESS && value < middle+STICKYNESS) {
      value = middle;
  }
  if ( abs(value - prev_value) > DEAD_ZONE ){
    controlChange(1,effect, map(value,0,1023,0,127) );
    prev_value = value;
    MidiUSB.flush();
  }
}

DigitalButton::DigitalButton(int pin, byte effect, byte effect_toggle, byte effect_long, byte effect_long_toggle) {
  this->effect = effect;
  this->effect_toggle = effect_toggle;
  this->effect_long = effect_long;
  this->effect_long_toggle = effect_long_toggle;
  this->pin = pin;
  pinMode(pin,INPUT_PULLUP);
}


void DigitalButton::checkStatus() {
  if (digitalRead(pin) == LOW && pressed == false) {
      pressed = true;
      long_pressed = false;
      debounce = millis();
    }
    if (pressed && (millis() - debounce <= 10) && digitalRead(pin) == HIGH) {
      pressed = false;
      return;
    }
    if (pressed && (millis() - debounce > 10) && digitalRead(pin) == HIGH) {
        if (long_pressed == true) {
          long_pressed = false;
        } else if (effect_toggle == 0) {
          controlChange(1,effect,1);
        } else if (toggled) {
          controlChange(1,effect,1);
          toggled = false;
        } else {
          controlChange(1,effect_toggle,1);
          toggled = true;
        }
      MidiUSB.flush();        
      pressed = false;
    }
    if (pressed && (millis() - debounce > LONG_INTERVAL) && (digitalRead(pin) == LOW) && (long_pressed == false)) {
      long_pressed = true;
      if (effect_long_toggle == 0) {
        controlChange(1,effect_long,1);
      } else if (long_toggled) {
        controlChange(1,effect_long,1);
        long_toggled = false;     
      } else {
        controlChange(1,effect_long_toggle,1);
        long_toggled = true;              
      }
      MidiUSB.flush();
    }
}

Mux::Mux(int pinSIG, int pinS0, int pinS1, int pinS2, int pinS3, int effects[16], int mid_deadzone[16]) {
  this->pinSIG = pinSIG;
  this->pinS0 = pinS0;
  this->pinS1 = pinS1;
  this->pinS2 = pinS2;
  this->pinS3 = pinS3;
  pinMode(pinSIG,INPUT);
  pinMode(pinS0,OUTPUT);
  pinMode(pinS1,OUTPUT);
  pinMode(pinS2,OUTPUT);
  pinMode(pinS3,OUTPUT);
  for (int i = 0; i < 16; i++) {
    this->effects[i] = effects[i];
    this->mid_deadzone[i] = effects[i];
  }
}

void Mux::checkStatus(byte selector){
  digitalWrite(pinS0, bitRead(selector,0));
  digitalWrite(pinS1, bitRead(selector,1));
  digitalWrite(pinS2, bitRead(selector,2));
  digitalWrite(pinS3, bitRead(selector,3));  
  int value = analogRead(pinSIG);

  if (value < STICKYNESS) {
    value = 0;
  } else if (value > 1023-STICKYNESS) {
    value = 1023;
  } else if (value > mid_deadzone-STICKYNESS && value < mid_deadzone+STICKYNESS) {
    value = mid_deadzone;
  }

  if (abs(value - prev_value[selector]) > DEAD_ZONE ) {
    byte midi_value = map(value,0,1023,0,127);
    controlChange(1,effects[selector],midi_value);
    #ifdef DEBUG
      Serial.print("Mux ");
      Serial.print(effects[selector]);
      Serial.print(", ");
      Serial.println(value);
    #endif
    prev_value[selector] = value;
    MidiUSB.flush();
  }
}
