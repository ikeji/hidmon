HIDmon Kai
===============

HIDmon Kai is a modified version of HIDmon.

This repository has these module.

1. Firmware for ATTiny2313.
2. Ruby module for control HIDmon Kai which is worked on Win32, Linux and mac.
3. AVR Writter program.

Goal
---------------

HIDmon Kai is

* USB-GPIO
* AVR Writter

Hardware
---------------
FIXME

Firmware
---------------

The firmware is based on original hidapx.
It's compiled by avr-gcc.

The size of firmware is restricted by flash size.
I added devtool directory for fix gcc's version.
`devtool/build.sh` script creates avr-gcc on devtool directory.
You can use this avr-gcc with add this dicrectory to PATH enviroment.

### Devtool directory

In ubuntu,
`devtool/build.sh` requires autogen package.
You can install by this command

    sudo apt-get install autogen

In cygwin,
`devtool/build.sh` requires wget, m4, libguile-dev package.
You can install by `setup.exe`

Host software
---------------

Host software library is written in Ruby.

### In ubuntu
It depends on libusb-ruby package.
You can install by this command

    sudo apt-get install libusb-ruby

By default, the permission of the device is opened for root only.
You can add follow file as `/dev/udev/rules.d/10-hidmon.rules`.

    SUBSYSTEM=="usb", ATTR{idVendor}=="16c0", ATTRS{idProduct}=="05df", MODE="0664", GROUP="plugdev"

### Windows
No additional package and administrator privilege is required.

Licence
---------------
GPL v2
