A simple project that transform an Arduino Leonardo (or similar boards based on ATmega32u4) in a mixing console for open source Mixxx DJ software. To compile, you'll need MIDIUSB and EncoderButton libraries. Both are downloadable from the official Arduino IDE's library manager.

After uploading the sketch and connecting the controls, you'll have to "train" Mixxx to recognize the various sliders and knobs: click "Learning Wizard" and then follow the instructions. Some of the commands aren't selectable from the interface, you'll need to use the drop down menu to select them (i.e. "scroll up/down the library"). An example configuration is in the folder of the same name. You have to copy the .xml into /home/<user>/.mixxx/controllers/ folder.

Be aware that the controller needs to be plugged in before starting Mixxx. If you enabled the debug, you have to open a serial console to start the device.

At the moment it supports these kinds of controls:
- Analog controls (knobs, sliders) need to be connected to an analog input.
- Rotary encoders, clk should be connected to an interrupt pin, dt to a digital input.
- Buttons (also on top of rotary encoders) need to be connected to digital input.

Buttons feature single and long press. They could have also two states that toggle (i.e. start/stop, etc.).
Analog controls have three "magnetic" positions: around 0, 1023 and middle (user defined) the reading has more stickyness (user definable). If you see that Mixxx's control is not in the middle when the potentiometer is at its half, alter the middle "magnetic" position.
They have also a dead zone that filters small variations in the readings. If you see that the controls are oscillating, increase the dead zone.

There are two experimental improvements:
- In the "console" folder there is a PCB made in Kicad, ready to be printed. Some of the footprints come from Digi-Key/digikey-kicad-library. The example configuration is trained on this PCB.
- If you need more analog controls, there is a version that features a multiplexer. I've tested with three potentiometers and worked, I've not tested it yet for a full console anyway.
