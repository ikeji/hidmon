@echo off
REM Written by senshu (2008/11/04)
REM Modified by senshu (2010/02/10)
pushd ..
echo ======= cleanup firmware =======
cd bin\firmware
call hexcleanup.bat
cd ..\..
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

rm -fv *.hex
REM make default (HIDaspx)
make clean
make
make install
make clean

REM make default (HIDaspx) for PRO version
REM make -f Makefile.pro clean
REM make -f Makefile.pro
REM make -f Makefile.pro install
REM make -f Makefile.pro clean

REM make HIDaspx (RC mode , MONITOR only)
REM make MAIN=main-small F_CPU=12800000 clean
REM make MAIN=main-small F_CPU=12800000 
REM make MAIN=main-small F_CPU=12800000  install
REM make MAIN=main-small F_CPU=12800000 clean

REM make HIDaspx (MONITOR only)
REM make MAIN=main-small clean
REM make MAIN=main-small
REM make MAIN=main-small install
REM make MAIN=main-small clean

REM make for libusb
make -f Makefile.unix clean
make -f Makefile.unix
make -f Makefile.unix install
make -f Makefile.unix clean
cd ..
echo ======= All update. =======
popd
avr-size ../bin/firmware/*.hex
REM upx -9 ../bin/hid*.exe
pause
