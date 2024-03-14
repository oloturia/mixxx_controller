#include "Controllers.h"

/* This is a sketch that it transforms your Arduino Leonardo or Leonardo like (i.e. boards with atmega32u4) in a MIDI console for interfacing
 * with Mixxx (https://mixxx.org). Perhaps you can also use an UNO but you have to sacrifice some pins in order to add a MIDI interface to your Arduino.
 * Once you have flashed the board, you have to train Mixxx in Preferences/Controllers menu.
 * Remember to plug the Arduino before launching Mixxx, otherwise it won't be detected.
 * 
 * This software is released under the Unlicense (see LICENSE for more info).
 * 
 * Last revision 27-dec-2023
 */

// Pin definition
/* Connect linear potentiometers (sliders and knobs) to Analog Inputs on your board.
 * On Leonardo, from A0 to A5 are on the left (with USB on top)
 * A6, A7, A8, A9, A10 and A11 are digital pins:
 *  4,  6,  8,  9,  10,     12.
 *  
 * Change the first value accordingly, the last one is the MIDI effect that is sent to
 * Mixxx, every control needs to have a unique number.
*/

//Analog controls pins and effects (effects have to be unique among all controls)
int analog_controls_pins[] = {A0,A1,A2,A3,A4,A5,A7,A10,A11};
int analog_controls_effects[] = {7,8,9,1,2,3,4,5,6};

/* Rotary Encorders need two pins. 
 * clk should be an interrupt. On Leonardo interrupts are on pin 0, 1, 2, 3 and 7.
 * dt are digital pins
 * rot_effect must be unique and also rot_effect+1 has to be
 * it's reserved for anticlockwise  
 */ 

const int rot_clk_pins[] = {2,3,0};
const int rot_dt_pins[] = {4,5,1};
const int rot_effect[] = {10,12,18};

/* Digital buttons are connected to a digital pin. If you use toggle, long press or long press toggle rembember that the MIDI message byte has to be unique.
 * long_inteval is the interval of long clicks, toggle and long_toggle (for long presses) send two different messages for odd and even strokes.
 * If effect is 0 the toggle, long press or long press toggle is disabled. 
 */

const int button_pins[] = {9,11,8,7,13};
const int button_effect[] = {14,15,16,17,20};
const int button_effect_toggle[] = {0,0,0,0,0};
const int button_effect_long[] {0,23,0,24,25};
const int button_effect_toggle_long[] {0,0,0,26,0};


AnalogControl analog_controls[] = {
  AnalogControl(analog_controls_pins[0],analog_controls_effects[0],520),
  AnalogControl(analog_controls_pins[1],analog_controls_effects[1],520),
  AnalogControl(analog_controls_pins[2],analog_controls_effects[2],520),
  
  AnalogControl(analog_controls_pins[3],analog_controls_effects[3],520),
  AnalogControl(analog_controls_pins[4],analog_controls_effects[4],520),
  AnalogControl(analog_controls_pins[5],analog_controls_effects[5],520),
  
  AnalogControl(analog_controls_pins[6],analog_controls_effects[6],520),
  AnalogControl(analog_controls_pins[7],analog_controls_effects[7],520),
  AnalogControl(analog_controls_pins[8],analog_controls_effects[8],520),
};

int active_analog_controls = sizeof(analog_controls)/sizeof(analog_controls[0]);

EncoderButton rotary_encoders[] = {
 EncoderButton(rot_clk_pins[0],rot_dt_pins[0]),
 EncoderButton(rot_clk_pins[1],rot_dt_pins[1]),
 EncoderButton(rot_clk_pins[2],rot_dt_pins[2]),
};

const int active_encoders = sizeof(rotary_encoders)/sizeof(rotary_encoders[1]);

void eb_Encoder(EncoderButton& eb) {
 int inc = eb.increment();
 if (inc > 0) {
  controlChange(1,eb.userId()+1,1);
 } else {
  controlChange(1,eb.userId(),1);
 }
 MidiUSB.flush();
}


DigitalButton switches[] = {
  DigitalButton(button_pins[0],button_effect[0],button_effect_toggle[0],button_effect_long[0],button_effect_toggle_long[0]),
  DigitalButton(button_pins[1],button_effect[1],button_effect_toggle[1],button_effect_long[1],button_effect_toggle_long[1]),
  DigitalButton(button_pins[2],button_effect[2],button_effect_toggle[2],button_effect_long[2],button_effect_toggle_long[2]),
  DigitalButton(button_pins[3],button_effect[3],button_effect_toggle[3],button_effect_long[3],button_effect_toggle_long[3]),
  DigitalButton(button_pins[4],button_effect[4],button_effect_toggle[4],button_effect_long[4],button_effect_toggle_long[4]),
};
const int active_switches = sizeof(switches)/sizeof(switches[0]);



void setup() {
  #ifdef DEBUG
    while(!Serial);
    Serial.begin(115200);
    Serial.println("RDY");
  #endif

  for (int i = 0; i < active_encoders; i++) {
    //pinMode(rot_clk_pins[i],INPUT);
    //pinMode(rot_dt_pins[i],INPUT);
    rotary_encoders[i].setUserId(rot_effect[i]);
    rotary_encoders[i].setEncoderHandler(eb_Encoder);
  }
    
}

void loop() {
  // Analog controls check.
  for (int i = 0; i < active_analog_controls; i++) {
    analog_controls[i].checkStatus();
  }

  // Switches check.
  for (int i = 0; i < active_switches; i++) {
    switches[i].checkStatus();
  }
  
  // Rotary encoders check.
  for (int i = 0; i < active_encoders; i++) {
    rotary_encoders[i].update();
  }


  
}
