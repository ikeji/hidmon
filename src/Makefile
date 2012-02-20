### Makefile for Borland C++
O = .obj
PROG = hidspx
#LIB =  libusb/lib/bcc/libusb.lib
LIB =
EXE  = $(PROG).exe
SRCS = avrspx.c hidasp.c hidasp_.c hwctrl.c rscr.c usbasp.c utils.c wronly.c libusb_dyn.c
#WILDARGS = c:\borland\bcc55\lib\wildargs$(O)
WILDARGS = c:\bcc55\lib\wildargs$(O)
OBJS = avrspx$(O) hidasp$(O) hwctrl$(O) rscr$(O) usbasp$(O) utils$(O) wronly$(O) libusb_dyn$(O)

RC = brc32
CC = bcc32
CFLAGS = -O2 -I. -DUSER_BOOKMARKS=1 -DTIME_DISPLAY=1 -DHIDASPX=1 -DAVRSPX=1 -Ilibusb/include
#CFLAGS = -O2 -I. -DHIDASPX=1 -DAVRSPX=1 -Ilibusb/include

LD = bcc32
#LDFLAGS = -mwindows -s -Wl,--subsystem,console
LDFLAGS =

#LDFLAGS =
# win app:
#	-mwindows
#
# win app with console:
#	-mwindows -Wl,--subsystem,console
#
# win app (which can be executed without cygwin1.dll):
#	-mwindows -mno-cygwin
#
# win app with console (which can be executed without cygwin1.dll):
#	-mwindows -mno-cygwin -Wl,--subsystem,console
#

.SUFFIXES : $(O) .c
.SUFFIXES : .coff .rc

ALL: $(EXE)

clean:
	rm -f $(RES) $(OBJS) $(EXE) *.tds timeit.dat icon.RES

.c$(O):
	$(CC) $(CFLAGS) -c $<

#.rc.coff:
#	$(RC) -i $< -o $@

$(EXE): $(OBJS) $(RES) $(LIB)
	$(LD) $(LDFLAGS) -e$(EXE)  $(WILDARGS) $(OBJS) $(RES) -lsetupapi $(LIB)
	$(RC) icon.rc $(EXE)

#icon$(O):	icon.rc
#	$(RC) -i icon.rc -o icon$(O)

install:
	install $(EXE) ../bin
###
avrspx$(O): avrspx.h usbasp.h hidasp.h
hidasp$(O): usbhid.h hidasp.h avrspx.h ../firmware/hidcmd.h
hidasp_$(O): usbhid.h hidasp.h avrspx.h ../firmware/hidcmd.h
hwctrl$(O): avrspx.h hwctrl.h
rscr$(O): avrspx.h
usbasp$(O): ./usb.h avrspx.h usbasp.h
utils$(O): avrspx.h
wronly$(O): avrspx.h
libusb_dyn$(O): libusb_dyn.c
