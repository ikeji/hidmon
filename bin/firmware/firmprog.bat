@echo off
echo Set FUSE, flash, serial infomations.
hidspx -d4 -fL0xff -fH0xdb -fX0xff main-12.hex %1
