# aek

![aek](imgur.com image replace me!)

*A short description of the keyboard/project*

* Keyboard Maintainer: [djvs](https://github.com/djvs)
* Hardware Supported: *The PCBs, controllers supported*
* Hardware Availability: *Links to where you can find this hardware*

Make example for this keyboard (after setting up your build environment):

    make aek:default

Flashing example for this keyboard:

    make aek:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).


# REAL NOTES

Do:

```
# install qmk and any needed python modules

qmk setup # to set up ~/qmk_firmware if it doesn't exist 

cp -rv /whatever/path/to/this/firmware/dir ~/qmk_firmware/keyboards/aek (or use ln -s) 

qmk flash -kb aek -km djvs 

# do mod + power key on the actual keyboard to reset

```
