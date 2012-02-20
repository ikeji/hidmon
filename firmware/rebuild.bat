@echo off
REM make HIDaspx
make clean
make -f Makefile.unix
make -f Makefile.unix install
REM make libusb
make clean
make
make install
