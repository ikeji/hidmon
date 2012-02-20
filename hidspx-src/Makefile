### Makefile.gcc
SRCS = avrspx.c hidasp.c hwctrl.c rscr.c usbasp.c utils.c wronly.c icon.rc
OBJS = avrspx.o hidasp.o hwctrl.o rscr.o usbasp.o utils.o wronly.o icon.o
PROG = hidspx
LIB =  libusb/lib/gcc/libusb.a
#RES  = $(PROG).coff
EXE  = $(PROG).exe

RC = windres
CC = gcc
CFLAGS = -O2 -I.  -DAVRSPX=1 -Ilibusb/include

LD = gcc
LDFLAGS = -mwindows -s -Wl,--subsystem,console

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

.SUFFIXES : .o .c
.SUFFIXES : .coff .rc

ALL: $(EXE)

clean:
	rm -f $(RES) $(OBJS) $(EXE) timeit.dat

.c.o:
	$(CC) $(CFLAGS) -c $<

#.rc.coff:
#	$(RC) -i $< -o $@

$(EXE): $(OBJS) $(RES) $(LIB)
	$(LD) $(LDFLAGS) -o $(EXE) $(OBJS) $(RES)  -lsetupapi $(LIB)

icon.o:	icon.rc
	$(RC) -i icon.rc -o icon.o

install:
	install $(EXE) c:/bin
