# Robot Hand Controller

An app for controlling a [Robot Nano Hand](https://robotnanohand.com/).  
This release is **calibrated to my own hand setup** and works only on **ARM64 Macs**.  
The source code is based on [Hamsa](https://gitlab.com/robot-nano-hand/hamsa/-/tree/master/src/cpp?ref_type=heads).

[Watch the demo](https://youtu.be/kCV66GzrXGE)


## Features

- Sliders to control each finger's curl and wiggle.
- Reset button to return the hand to a neutral position.
- Preset hand poses: peace, fist, rock, ILY, OK, etc.
- Automatic warning and exit if the hand is not connected.


## Installation
> ⚠️ Built for **ARM64 macOS**. 
1. Download the `.app` release from this repository.
2. Unzip the release package.
3. Connect your [Robot Nano Hand](https://robotnanohand.com/).
4. Launch the app.

If you wish to use different calibration settings or run on another OS, you must build it from the Qt project files included in this repository.


## Notes

- This is a **personal build**; slider ranges and preset poses are tuned to my hand.
- Built with Qt 6 and C++.


## License

This project is for personal use. Any external code from Hamsa or Feetech follows the Apache 2.0 license requirements.



