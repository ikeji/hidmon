# Name: Makefile
# Project: HIDmon Kai
# Author: ikeji
# License: GNU GPL v2 (see License.txt)

# Config

F_CPU = 12000000
#F_CPU = 12800000
#F_CPU = 16000000
#F_CPU = 20000000
TARGET = main

# default build target

default: $(TARGET).hex

# dispatch all build target

all: main-12.hex main-16.hex main-20.hex \
	main-small-12.hex main-small-rc.hex main-small-16.hex main-small-20.hex

main-12.hex:
	$(MAKE) F_CPU=12000000 TARGET=main clean main.hex
	cp main.hex main-12.hex
	cp main.elf main-12.elf

main-16.hex:
	$(MAKE) F_CPU=16000000 TARGET=main clean main.hex
	cp main.hex main-16.hex
	cp main.elf main-16.elf

main-20.hex:
	$(MAKE) F_CPU=20000000 TARGET=main clean main.hex
	cp main.hex main-20.hex
	cp main.elf main-20.elf

main-small-12.hex:
	$(MAKE) F_CPU=12000000 TARGET=main-small clean main-small.hex
	cp main-small.hex main-small-12.hex
	cp main-small.elf main-small-12.elf

main-small-rc.hex:
	$(MAKE) F_CPU=12800000 TARGET=main-small clean main-small.hex
	cp main-small.hex main-small-rc.hex
	cp main-small.elf main-small-rc.elf

main-small-16.hex:
	$(MAKE) F_CPU=16000000 TARGET=main-small clean main-small.hex
	cp main-small.hex main-small-16.hex
	cp main-small.elf main-small-16.elf

main-small-20.hex:
	$(MAKE) F_CPU=20000000 TARGET=main-small clean main-small.hex
	cp main-small.hex main-small-20.hex
	cp main-small.elf main-small-20.elf

.PHONY: allclean
allchecksize: all
	-sh checksize main-12.elf
	-sh checksize main-16.elf
	-sh checksize main-20.elf
	-sh checksize main-small-12.elf
	-sh checksize main-small-rc.elf
	-sh checksize main-small-16.elf
	-sh checksize main-small-20.elf

.PHONY: allclean
allclean: clean
	rm -f *.hex *.elf

# Constants

DEVICE = attiny2313

CFLAGS = $(DEFINES) \
				 -Wall \
				 -Os \
				 -Iusbdrv -I. \
				 -DDEBUG_LEVEL=0 \
				 -DF_CPU=$(F_CPU) \
				 -D__AVR_LIBC_DEPRECATED_ENABLE__ \
				 -mmcu=$(DEVICE) \
				 -fdata-sections \
				 -ffunction-sections \
				 -ffreestanding \
				 -fno-schedule-insns2 \
				 -fno-tree-scev-cprop \
				 -fno-split-wide-types \
				 -Wl,--relax,--gc-sections -nostdlib

ASFLAGS = $(CFLAGS)

OBJS = \
			 $(TARGET).o \
			 crt.o \
			 delay.o \
			 usbdrv/usbdrv.o \
			 usbdrv/usbdrvasm.o

CC=avr-gcc

version:
	avr-gcc -v

.SUFFIXES: .hex .elf

# build

hex: $(TARGET).hex

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

.elf.hex:
	avr-objcopy -j .text -j .data -O ihex $< $@
	-sh checksize $<

.PHONY: clean
clean:
	rm -f $(TARGET).hex $(TARGET).elf *.o usbdrv/*.o

# Flash

PROGRAMMER=hidspx

FUSE_L  = 0xBF
FUSE_H  = 0xDB

program: flash fuse

flash: $(TARGET).hex
	$(PROGRAMMER) $<

fuse:
	$(PROGRAMMER) -FL$(FUSE_L) -FH$(FUSE_H)

depend:
	$(CC) $(CFLAGS) -MM *.c *.S usbdrv/*.S usbdrv/*.c > makefile.depend

include makefile.depend
