A simple project that transform an Arduino Leonardo (or similar boards based on ATmega32u4) in a mixing console for open source Mixxx DJ software.

Attach some potentiometers to analogue inputs (you can alter the inputs in the first part of the sketch - there is no real difference between knobs and sliders, it's just the structs name), plug the Arduino then open Mixxx and open the preferences. You should see a new entry under "Controllers".

After that you have to "train" Mixxx to recognize the various sliders and knobs: click "Learning Wizard" and then follow the instructions!

To do: add some digital inputs for buttons and switches.
