@echo off
@echo ATtiny2313 setting FUSE (RC mode).
hidspx -qtiny2313 -d10 -e
hidspx -qtiny2313 -d10 -FL0xA4 -FH0xDB -FX0xFF
@echo Write firmware (monitor only)
hidspx -qtiny2313 -d1 main-small-RC.hex
