@echo off
make clean
make MAIN=main-small clean
make -f Makefile.unix clean
