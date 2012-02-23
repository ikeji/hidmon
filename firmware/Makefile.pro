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

CFLAGS = $(DEFINES) -Iusbdrv -I. -DPRO -DDEBUG_LEVEL=0 -DF_CPU=$(F_CPU)

#OBJS = $(TARGET).o usbdrv/usbdrv.o usbdrv/usbdrvasm.o 
OBJS = $(TARGET).o crt.o delay.o usbdrv/usbdrv.o usbdrv/usbdrvasm.o 

COMPILE = avr-gcc -Wall -Os $(CFLAGS) -mmcu=$(DEVICE)

############################## ATtiny2313 ##############################
#Low: 10111111 (0xBF)
#     ||||++++-- CKSEL[3:0] �V�X�e���N���b�N�I��
#     ||++-- SUT[1:0] �N������
#     |+-- CKOUT (0:PD2�ɃV�X�e���N���b�N���o��)
#     +-- CKDIV8 �N���b�N���������l (1:1/1, 0:1/8)
#
#High:11-11011 (0xDB)
#     |||||||+-- RSTDISBL (RESET�s�� 1:�L��, 0:����(PA2))
#     ||||+++-- BODLEVEL[2:0] (111:Off, 110:1.8, 101:2.7, 100:4.3)
#     |||+-- WDTON (WDT 0:�펞ON, 1:�ʏ�)
#     ||+-- SPIEN (1:ISP�֎~, 0:ISP����) ��Parallel���̂�
#     |+-- EESAVE (������EEPROM�� 1:����, 0:�ێ�)
#     +-- DWEN (On-Chip�f�o�b�O 1:����, 0:�L��)
#
#Ext: -------1 (0xFF)
#            +-- SPMEN (SPM���� 1:����, 0:�L��)
#
default: $(TARGET)-$(OSC)-pro.hex

# symbolic targets:
help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "make hex ......... to build main.hex for HIDaspx hardware"
	@echo "make program ..... to flash fuses and firmware"
	@echo "make fuse ........ to flash the fuses"
	@echo "make flash ....... to flash the firmware"
	@echo "make clean ....... to delete objects and hex file"

hex: $(TARGET)-$(OSC)-pro.hex
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
	rm -f *.o *.dis *.lst *.cof *.map *.elf time*.dat usbdrv/*.o usbdrv/oddebug.s usbdrv/usbdrv.s

install:
	cp -p $(TARGET)-$(OSC)-pro.hex ../bin/firmware

# file targets:
$(TARGET)-$(OSC)-pro.elf:	$(OBJS)
	$(COMPILE) -o $(TARGET)-$(OSC)-pro.elf $(OBJS) -nostdlib
#	$(COMPILE) -o $(TARGET)-$(OSC)-pro.elf $(OBJS)

$(TARGET)-$(OSC)-pro.hex:	$(TARGET)-$(OSC)-pro.elf
	rm -f $(TARGET)-$(OSC)-pro.hex $(TARGET)-$(OSC)-pro.eep.hex
	avr-objcopy -j .text -j .data -O ihex $(TARGET)-$(OSC)-pro.elf $(TARGET)-$(OSC)-pro.hex
	avr-size --mcu=$(DEVICE) $(TARGET)-$(OSC)-pro.elf
#	sh checksize main.elf 2048 128
#	checksize main.elf 8192 1024
# do the checksize script as our last action to allow successful compilation
# on Windows with WinAVR where the Unix commands will fail.

disasm:	$(TARGET)-$(OSC)-pro.elf
	avr-objdump -d $(TARGET)-$(OSC)-pro.elf

cpp:
	$(COMPILE) -E $(TARGET).c

###
$(TARGET)-$(OSC)-pro.o: $(TARGET).c usbconfig.h hidconfig.h hidcmd.h osctune.h\
 usbdrv/usbdrv.h usbdrv/usbportability.h
crt.o:	crt.S
delay.o:	delay.S
