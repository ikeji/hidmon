@echo off
make clean
make
make install
REM
make -f Makefile.gcc clean
make -f Makefile.gcc
make -f Makefile.gcc install
