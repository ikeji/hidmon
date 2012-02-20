@echo off
REM Written by senshu (2008/11/04)
REM Modified by senshu (2010/02/10)
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
echo ======= All update. =======
popd
avr-size ../bin/firmware/*.hex
REM upx -9 ../bin/hid*.exe
pause
