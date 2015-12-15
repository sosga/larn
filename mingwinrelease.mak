CC=gcc
LIBS= -Os -Wall -fomit-frame-pointer -DWINDOWS -Wl,-O1 -lm win/pdcurses.a win/panel.a 
DEPS=*.h
STRIP_BINARY=yes
larn: $(DEPS) *.c
	$(CC) *.c -o larn1250 $(LIBS)
