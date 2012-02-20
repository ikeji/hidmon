@echo off
@echo ATtiny2313 setting FUSE (RC mode).
hidspx -d10 -e
hidspx -d10 -FL0xA4 -FH0xDB -FX0xFF
@echo Write firmware (monitor only)
hidspx -d1 main-small-RC.hex
