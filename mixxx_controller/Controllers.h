#ifndef CONTROLLERS_H
#define CONTROLLERS_H
#include <EncoderButton.h>
#include "Arduino.h"
#include "MIDIUSB.h"

// Uncomment this line if you want pin and values printed on the serial, remember to open a console because it waits until a connection is established.
//#define DEBUG

// Analog controls (sliders and potentiometers), the deadzone is the smallest increment that the control must have to trigger the effect.
#define DEAD_ZONE 15

// Stickyness is a deazone that lies around extreme positions (0-1023) and the middle (511, but trimmable) of readings.
#define STICKYNESS 30

// Time, in ms, needed for a long press
#define LONG_INTERVAL 1000

void noteOn(byte channel, byte pitch, byte velocity);
void noteOff(byte channel, byte pitch, byte velocity);
void controlChange(byte channel, byte control, byte value);

class AnalogControl {
  private:
    int pin;
    byte effect;
    int middle;
    int value;
    int prev_value;
  public:
    AnalogControl(int pin, byte effect, int middle);
    void checkStatus();
};

class DigitalButton {
  private:
    int pin;
    bool pressed;
    long debounce;
    long countdown;
    byte effect;
    byte effect_toggle;
    bool toggled;
    byte effect_long;
    byte effect_long_toggle;
    bool long_toggled;
    bool long_pressed;
  public:
    DigitalButton(int pin, byte effect, byte effect_toggle, byte effect_long, byte effect_long_toggle);
    void checkStatus();
};

#endif
