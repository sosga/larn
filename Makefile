# Configuration options
#  LARNHOME is the directory where the larn data files will be installed.
#  BINDIR is the directory where the larn binary will be installed.

LARNHOME = /usr/user/student/dave/games/larn
BINDIR = /usr/user/student/dave/games/larn
CC= cc

# -ltermlib may need to be changed to -ltermcap on some systems
TERMLIB= -ltermlib

# Available compile time options:
#   WIZID=xxx  - this is the userid (or playerid) of the wizard.  Default is
#					zero (superuser), which disables all wizard functions.
#					Players must have this userid (or playerid) in order to
#					become the non-scoring wizard player.  Definition of WIZID
#					to non-zero will enable the special wizard debugging
#					commands.  For root to become wizard, use WIZID= -1.
#   EXTRA      - incorporates code to gather additional performance statistics
#   TIMECHECK  - incorporates code to disable play during working hours (8-5)
#   SYSV       - use system III/V (instead of V7) type ioctl calls
#   BSD        - use BSD specific features (mostly timer and signal stuff)
#   BSD4.1     - use BSD4.1 to avoid some 4.2 dependencies (must be used with
#					BSD above; do not mix with SYSV)
#   HIDEBYLINK - if defined, the program attempts to hide from ps
#   DOCHECKPOINTS - if not defined, checkpoint files are periodically written
#                   by the larn process (no forking) if enabled in the .larnopts
#					description file.  Checkpointing is handy on an unreliable
#					system, but takes CPU. Inclusion of DOCHECKPOINTS will cause
#					fork()ing to perform the checkpoints (again if enabled in
#					the .larnopts file).  This usually avoids pauses in larn
#					while the checkpointing is being done (on large machines).
#   SAVEINHOME - put save files in users HOME instead of LARNHOME the as default
#	VER        - This is the version of the software, example:  12
#	SUBVER     - This is the revision of the software, example:  1
#	FLUSHNO=#  - Set the input queue excess flushing threshold (default 5)
#	NOVARARGS  - Define for systems that don't have varargs (a default varargs
#					will be used).
#	MACRORND   - Define to use macro version of rnd() and rund() (fast & big)
#	UIDSCORE   - Define to use user id's to manage scoreboard.  Leaving this out
#					will cause player id's from the file ".playerids" to be used
#					instead.  (.playerids is created upon demand).  Only one
#					entry per id # is allowed in each scoreboard
#					(winning & non-winning).
#	VT100	  - Compile for using vt100 family of terminals.  Omission of this
#					define will cause larn to use termcap, but it will be MUCH
#					slower due to an extra layer of output interpretation. 
#					Also, only VT100 mode allows 2 different standout modes,
#					inverse video, and bold video.  And only in VT100 mode is
#					the scrolling region of the terminal used (much nicer than
#					insert/delete line sequences to simulate it, if VT100 is
#					omitted).
#	NONAP	  - This causes napms() to return immediately instead of delaying
#					n milliseconds.  This define may be needed on some systems
#					if the nap stuff does not work correctly (possible hang).
#					nap() is primarilly used to delay for effect when casting
#					missile type spells.
#
#	BSD2_9		A local CSU Stanislaus flag.  So far it is set around
#			unsigned long definitions -- Not implimented in 2.9.
#			dave.
#

OPTIONS = -DEXTRA -DBSD -DBSD4.1 -DVER=12 -DSUBVER=0 -DTIMECHECK -DWIZID=190 -DHIDEBYLINK -DBSD2_9
#
# dave:  added -DBSD4.1  -DBSD2_9
#	 changed -DWIZID=1000 to 190
#	 removed -DVT100
#
# End of configurable make options
########################################################################
#set up overlays
#dave:	4 segments to the overlay.  Right now, there is no true logic 
#	to what goes where.  I just took a quick guess -- therefore,
#	there is probably some/much thrashing going on.

OVBASE= config.o data.o display.o fortune.o global.o help.o io.o main.o\
	monster.o nap.o object.o regen.o savelev.o signal.o strings.o
OV0= moreobj.o movem.o tok.o
OV1= bill.o scores.o
OV2= diag.o
OV3= store.o
OV4= create.o

OBJS = $(OVBASE) $(OV0) $(OV1) $(OV2) $(OV3) $(OV4)
#
#OBJS= main.o object.o create.o tok.o display.o global.o data.o io.o monster.o\
#	store.o diag.o help.o config.o nap.o bill.o scores.o signal.o moreobj.o\
#	movem.o regen.o fortune.o savelev.o
#
DOTFILES= .larn.help .larnmaze .larnopts .lfortune

#FLAGS= -O $(OPTIONS) -DLARNHOME=\"$(LARNHOME)/\"
#new csustan stuff
FLAGS= -O -V $(OPTIONS) -DLARNHOME=\"$(LARNHOME)/\"

#larn: larn12.0
#
#larn12.0: $(OBJS) 
#	$(CC) $(FLAGS) $(OBJS) -o larn12.0 $(TERMLIB)
#	size larn12.0
#
larn:	xstr
	mv xs.c strings.c
	larn12.0

larn12.0:  $(OBJS)
	ld -i -X -x -o larn12.0 /lib/crt0.o $(OVBASE)\
	   -Z $(OV0) -Z $(OV1) -Z $(OV2) -Z $(OV3) -Z $(OV4)\
	   -L $(TERMLIB) -lovc
	size larn12.0
	
all: larn install

#.c.o:	$<
#	$(CC) -c $(FLAGS) $*.c
#	chmod 0600 $*.o
#

.c.o:	$<
	-cc -P $(FLAGS) $*.c
	-xstr -c $*.i
	-cc -c $(FLAGS) x.c
	-mv x.o larnx/$*.o
	-mv x.c larnx/$*.c
	-rm $*.i

obj:	$(OBJS)

noxstr:
	-cc -c $(FLAGS) bill.c
	-cp bill.o larnx
	-cc -c $(FLAGS) create.c
	-cp create.o larnx
	-cc -c $(FLAGS) diag.c
	-cp diag.o larnx
	-cc -c $(FLAGS) moreobj.c
	-cp moreobj.o larnx
	-cc -c $(FLAGS) movem.c
	-cp movem.o larnx
	-cc -c $(FLAGS) scores.c
	-cp scores.o larnx
	-cc -c $(FLAGS) store.c
	-cp store.o larnx
	-cc -c $(FLAGS) tok.c
	-cp tok.o larnx

$(OBJS): header.h

install:
#	if test ! -d $(LARNHOME) ; then mkdir $(LARNHOME); fi
	-chmod a+w $(LARNHOME)
	cp larn12.0 $(BINDIR)/larn
	cp $(DOTFILES) $(LARNHOME)
	-chmod 700 $(LARNHOME)/larn

