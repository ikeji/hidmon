@echo off
echo *** For HIDaspx firmware (libusb) ***
echo === Set FUSE data. === 
hidspx -qtiny2313 -d10 -fL0xFF -fH0xDB -fX0xFF
echo.
echo ===  Write flash and serial infomations. === 
hidspx -qtiny2313 -d1 main-12.hex %1
