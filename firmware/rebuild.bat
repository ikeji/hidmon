@echo off
REM make default (HIDaspx)
make clean
make
make install

REM make HIDaspx (MONITOR only)
make MAIN=main-small F_CPU=12800000 clean
make MAIN=main-small F_CPU=12800000 
make MAIN=main-small F_CPU=12800000  install

REM make HIDaspx (MONITOR only)
make MAIN=main-small clean
make MAIN=main-small
make MAIN=main-small install

REM make for libusb
make -f Makefile.unix clean
make -f Makefile.unix
make -f Makefile.unix install
