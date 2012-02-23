# Name: Makefile
# Project: HIDaspx
# Author: iruka & senshu
# Creation Date: 2008-09-02
# Tabsize: 4
# Copyright: (c) 2006 by OBJECTIVE DEVELOPMENT Software GmbH
# License: GNU GPL v2 (see License.txt) or proprietary (CommercialLicense.txt)
# This Revision: $Id: Makefile 566 2008-04-26 14:21:47Z cs $

# GNU make only
#DEVICE = atmega8
DEVICE = attiny2313
F_CPU = 12000000
#F_CPU = 16000000

OSC = 16
ifeq ($(F_CPU),12000000)
 OSC = 12
endif

ifeq ($(F_CPU),12800000)
 OSC = RC
endif

MAIN = main
TARGET = $(MAIN)

FUSE_L  = 0xBF
FUSE_H  = 0xDB

#PORT = avrdoper
#PROGRAMMER = stk500v2
#AVRDUDE = avrdude -c $(PROGRAMMER) -P $(PORT) -p $(DEVICE)
PROGRAMMER = avrspx

CFLAGS = $(DEFINES) -Iusbdrv -I. -DDEBUG_LEVEL=0 -DF_CPU=$(F_CPU)

#OBJS = $(TARGET).o usbdrv/usbdrv.o usbdrv/usbdrvasm.o 
OBJS = $(TARGET).o crt.o delay.o usbdrv/usbdrv.o usbdrv/usbdrvasm.o 

COMPILE = avr-gcc -Wall -Os $(CFLAGS) -mmcu=$(DEVICE)

############################## ATtiny2313 ##############################
#Low: 10111111 (0xBF)
#     ||||++++-- CKSEL[3:0] システムクロック選択
#     ||++-- SUT[1:0] 起動時間
#     |+-- CKOUT (0:PD2にシステムクロックを出力)
#     +-- CKDIV8 クロック分周初期値 (1:1/1, 0:1/8)
#
#High:11-11011 (0xDB)
#     |||||||+-- RSTDISBL (RESETピン 1:有効, 0:無効(PA2))
#     ||||+++-- BODLEVEL[2:0] (111:Off, 110:1.8, 101:2.7, 100:4.3)
#     |||+-- WDTON (WDT 0:常時ON, 1:通常)
#     ||+-- SPIEN (1:ISP禁止, 0:ISP許可) ※Parallel時のみ
#     |+-- EESAVE (消去でEEPROMを 1:消去, 0:保持)
#     +-- DWEN (On-Chipデバッグ 1:無効, 0:有効)
#
#Ext: -------1 (0xFF)
#            +-- SPMEN (SPM命令 1:無効, 0:有効)
#
default: $(TARGET)-$(OSC).hex

# symbolic targets:
help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "make hex ......... to build main.hex for HIDaspx hardware"
	@echo "make program ..... to flash fuses and firmware"
	@echo "make fuse ........ to flash the fuses"
	@echo "make flash ....... to flash the firmware"
	@echo "make clean ....... to delete objects and hex file"

hex: $(TARGET)-$(OSC).hex
program: flash fuse

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.S:
	$(COMPILE) -S $< -o $@

flash:
#	$(AVRDUDE) -U flash:w:$(TARGET).hex:i
	$(PROGRAMMER) -d1 $(TARGET).hex

#usbaspload:
#	$(MAKE) flash PROGRAMMER=usbasp DEVICE=atmega168

fuse:
#	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
#		{ echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
#	$(AVRDUDE) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m
	$(PROGRAMMER) -d10 -FL$(FUSE_L) -FH$(FUSE_H)

clean:
	rm -f $(TARGET)-$(OSC).hex *.o *.dis *.lst *.cof *.map *.elf time*.dat usbdrv/*.o usbdrv/oddebug.s usbdrv/usbdrv.s

install:
	cp -p $(TARGET)-$(OSC).hex ../bin/firmware

# file targets:
$(TARGET)-$(OSC).elf:	$(OBJS)
	$(COMPILE) -o $(TARGET)-$(OSC).elf $(OBJS) -nostdlib
#	$(COMPILE) -o $(TARGET)-$(OSC).elf $(OBJS)

$(TARGET)-$(OSC).hex:	$(TARGET)-$(OSC).elf
	rm -f $(TARGET)-$(OSC).hex $(TARGET)-$(OSC).eep.hex
	avr-objcopy -j .text -j .data -O ihex $(TARGET)-$(OSC).elf $(TARGET)-$(OSC).hex
	avr-size --mcu=$(DEVICE) $(TARGET)-$(OSC).elf
#	sh checksize main.elf 2048 128
#	checksize main.elf 8192 1024
# do the checksize script as our last action to allow successful compilation
# on Windows with WinAVR where the Unix commands will fail.

disasm:	$(TARGET)-$(OSC).elf
	avr-objdump -d $(TARGET)-$(OSC).elf

cpp:
	$(COMPILE) -E $(TARGET).c

###
$(TARGET)-$(OSC).o: $(TARGET).c usbconfig.h hidconfig.h hidcmd.h osctune.h\
 usbdrv/usbdrv.h usbdrv/usbportability.h
crt.o:	crt.S
delay.o:	delay.S
