# Little Sun Gazer

## This fork

Fork of https://github.com/dr-mod/little-sun-gazer/

Includes :

* Patch from https://github.com/dr-mod/little-sun-gazer/pull/3, thanks @tvoverbeek
* Modifications suggested by https://github.com/dr-mod/little-sun-gazer/issues/1#issuecomment-1155629352, thanks @marcus-e-e
* Modifications to reference libraries in parallel folders rather than copying files from them into this project. See Build section below.
* Modification to change Font48 to Font24 as it isn't clear where to find Font48

## Build
### Pre-requirements
This is for the Mac:
Blah blah
###
```
git clone https://github.com/alexdyas/little-sun-gazer.git
git clone https://github.com/waveshare/Pico_ePaper_Code.git --recurse-submodules
git clone https://github.com/raspberrypi/pico-sdk.git --recurse-submodules
git clone https://github.com/raspberrypi/pico-extras.git --recurse-submodules
cd little-sun-gazer
./run_build.sh
```

A miniature device depicting the relative position of the Sun and moon.

![pic](docs/pic.jpg)

### Hardware
* Raspberry Pi Pico
* Precision RTC Module (DS3231)
* Waveshare e-Paper 3.7
* Pimoroni LiPo SHIM
* Li-Po 2000mAh 103450

### Case
A 3d printable case for this project will be [here](https://www.thingiverse.com/dr2mod/designs).

### Support the project
If you would like to support what I do and keep me caffeinated, you can do it here:

[!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://www.buymeacoffee.com/drmod)

### Disclaimer 
This is not a finished project, but at the moment I don't have access to required hardware to finish it.