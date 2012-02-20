@echo off
make clean
make
make install
REM
make -f Makefile.bcc clean
make -f Makefile.bcc
make -f Makefile.bcc install
