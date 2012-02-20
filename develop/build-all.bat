@echo off
REM Written by senshu (2008/11/04)
pushd ..
echo ======= hidspx.exe build =======
cd src
make clean
make
make install
make clean
REM
make -f Makefile.gcc clean
make -f Makefile.gcc
make -f Makefile.gcc install
make -f Makefile.gcc clean
cd ..
echo ======= firmware build =======
cd firmware
REM make default (HIDaspx)
make clean
make
make install

REM make default (HIDaspx)
REM make -f Makefile.pro clean
REM make -f Makefile.pro
REM make -f Makefile.pro install

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
cd ..
echo ======= All update. =======
popd
avr-size ../bin/firmware/*.hex
REM upx -9 ../bin/hid*.exe
pause
