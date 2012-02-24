This is the README file for HIDaspx.

HIDaspx is a USB in-circuit programmer for Atmel AVR controllers. 
It simply consists of an ATtiny2313 and a couple of passive components.
The programmer uses a firmware-only USB driver, no special USB controller 
is needed.

Features:
- Works under platforms. Windows 2000/XP/Vista are tested.
- No special controllers or smd components are needed.
- Programming speed is up to 4kBytes/sec (Write Only).
- SCK option to support targets with low clock speed (< 1.5MHz).


LICENSE

HIDaspx is distributed under the terms and conditions of the GNU GPL
version 2 (see "firmware/usbdrv/License.txt" for details).

HIDaspx is built with AVR USB driver by OBJECTIVE DEVELOPMENT GmbH. 
See "firmware/usbdrv/" for further information.


LIMITATIONS

Hardware:
This package includes a circuit diagram. 
This circuit can be used for programming 5V or 3.3V target systems. 

Firmware:
The firmware dosn't support USB Suspend Mode. 

USE PRECOMPILED VERSION

Firmware:
Flash "bin/firmware/main.hex" to the used controller with a working programmer 
(e.g. with avrspx, uisp, ...). 
HIDaspx firmware update function.
You have to change the fuse bits for external crystal (see "make fuses").

Windows:
Start Windows and connect HIDaspx to the system. 
Now you can run hidspx. Examples:
1. Enter target AVR chip connected to the programmer:
   hidspx -ph -r <Enter>
2. Write main.hex to the flash of an ATtiny2313:
   hidspx main.hex

BUILDING AND INSTALLING FROM SOURCE CODE

Firmware:
To compile the firmware
1. install the GNU toolchain for AVR microcontrollers (avr-gcc, avr-libc),
2. change directory to firmware/
3. run "make main.hex"
4. flash "main.hex" to the ATtiny2313. 

FILES IN THE DISTRIBUTION

Readme.txt ...................... The file you are currently reading
firmware ........................ Source code of the controller firmware
firmware/usbdrv ................. AVR USB driver by Objective Development
firmware/usbdrv/License.txt ..... Public license for AVR USB driver and HIDaspx
circuit ......................... Circuit diagram in jpg and png format
hidspx-src ...................... hidspx source code
bin ............................. Precompiled tool (hidspx.exe)
bin/firmware .................... Precompiled firmware


MORE INFORMATION

For more information on HIDaspx and it's components please visit the
following URLs:

HIDaspx ........................ http://www-ice.yamagata-cit.ac.jp/ken/senshu/sitedev/index.php?AVR%2FHIDasp
Firmware-only AVR USB driver ... http://www.obdev.at/products/avrusb/

2008-09-29 Hiroyuki Senshu <senshu(at)astro.yamagata-cit.ac.jp>
