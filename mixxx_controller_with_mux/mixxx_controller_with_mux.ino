#include "Controllers.h"

/* This is a sketch that it transforms your Arduino Leonardo or Leonardo like (i.e. boards with atmega32u4) in a MIDI console for interfacing
 * with Mixxx (https://mixxx.org). Perhaps you can also use an UNO but you have to sacrifice some pins in order to add a MIDI interface to your Arduino.
 * Once you have flashed the board, you have to train Mixxx in Preferences/Controllers menu.
 * Remember to plug the Arduino before launching Mixxx, otherwise it won't be detected.
 * 
 * This software is released under the Unlicense (see LICENSE for more info).
 * 
 * This version allow the use of multiplexers for analog inputs, this feature is not tested yet.
 * If you have doubts, use mixxx_controller.ino instead.
 * 
 * Last revision 27-mar-2024
 */

/* You can alter some library wide parameters in Controllers.h
 * DEBUG enables serial printing of MIDI messages
 * DEAD_ZONE contains the smallest increment that the control must have to trigger the effect.
 * STICKYNESS is a deazone that lies around extreme positions (0-1023) and the middle of readings.
 * TIME_INTERVAL is the time, in ms, needed for a long press
*/

// Pin definition
// Analog knobs and sliders
/* Connect linear potentiometers (sliders and knobs) to Analog Inputs on your board.
 * On Leonardo, from A0 to A5 are on the left (with USB on top)
 * A6, A7, A8, A9, A10, A11 are digital pins:
 *  4,  6,  8,  9,  10,  12.
 *  
 * Change the first value accordingly, the last one is the MIDI effect that is sent to
 * Mixxx, every control needs to have a unique number.
 * 
 * MID-DEADZONE is the middle of the controller. Usually is 511, but it's trimmable.
*/

// Define every control as AnalogControl(PIN,EFFECT,MID-DEADZONE)
AnalogControl analog_controls[] = {
//  AnalogControl(A0,7,520),

};

int active_analog_controls = sizeof(analog_controls)/sizeof(analog_controls[0]);

// Rotary encoders
/* Rotary Encorders need two pins. 
 * clk should be an interrupt. On Leonardo interrupts are on pin 0, 1, 2, 3 and 7.
 * dt are digital pins
 * effect and effect+1 must be unique
 * the +1 is reserved for anticlockwise  
 */ 

// Define every control as {EncoderButton(CLK,DT), EFFECT}
RotaryEncoderControl rotary_encoders[] = {
  //{EncoderButton(2,4),10},
  
};

const int active_encoders = sizeof(rotary_encoders)/sizeof(rotary_encoders[1]);

// Digital buttons
/* Digital buttons are connected to a digital pin. If you use toggle, long press or long press toggle rembember that the MIDI message byte has to be unique.
 * toggle effect and long toggle effect (for long presses) send two different messages for odd and even strokes.
 * If effect is 0 the toggle, long press or long press toggle is disabled.
 */

// Define every control as DigitalButton(PIN,PRESS EFFECT, PRESS TOGGLE EFFECT (0=disabled), LONG PRESS EFFECT, LONG PRESS TOGGLE EFFECT (0=disabled)
DigitalButton buttons[] = {
//  DigitalButton( 9,14,0, 0, 0),

};
const int active_buttons = sizeof(buttons)/sizeof(buttons[0]);

//Callback function for the encoders
void eb_Encoder(EncoderButton& eb) {
 int inc = eb.increment();
 if (inc > 0) {
  controlChange(1,eb.userId()+1,1);
 } else {
  controlChange(1,eb.userId(),1);
 }
 MidiUSB.flush();
}

/* Multiplexers allow multiple analog inputs using only 1 analog input and 4 digital outputs.
 * pinSIG is the input pin for the signal, pinS0 to pinS3 are the digital outputs that select
 * the analog channel that will be connected with SIG.
 * 
 */

// Define every Multiplexer as 
// (SIGNAL PIN, S0, S1, S2, S3, EFFECT (array of 16), MIDDLE DEADZONE (array of 16) ) 

int mux0[16] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
int midzones0[16] = {520,520,520,520,520,520,520,520,520,520,520,520,520,520,520,520};

Mux mux_inputs[] = {
  Mux(A0, 4, 5, 6, 7, mux0, midzones0)
};

const int active_mux_inputs = sizeof(mux_inputs)/sizeof(mux_inputs[0]);


void setup() {
  #ifdef DEBUG
    while(!Serial);
    Serial.begin(115200);
    Serial.println("RDY");
  #endif

  for (int i = 0; i < active_encoders; i++) {
    rotary_encoders[i].rotary_encoder.setUserId(rotary_encoders[i].effect);
    rotary_encoders[i].rotary_encoder.setEncoderHandler(eb_Encoder);
  }   
}

void loop() {

  // Analog controls check
  for (int i = 0; i < active_analog_controls; i++) {
    analog_controls[i].checkStatus();
  }

  // Buttons check
  for (int i = 0; i < active_buttons; i++) {
    buttons[i].checkStatus();
  }
  
  // Rotary encoders check
  for (int i = 0; i < active_encoders; i++) {
    rotary_encoders[i].rotary_encoder.update();
  }
  
  // Analog signal from multiplexer check.
  for (int i = 0; i < active_mux_inputs; i++) {
    for (int act = 0; act < 16; act++) {
      mux_inputs[i].checkStatus(act);
    }
  }
 
}
