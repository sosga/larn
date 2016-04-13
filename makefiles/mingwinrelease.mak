CC = gcc
LIBS = -Os -Wall -fomit-frame-pointer -DWINDOWS -DMULTIPLE_SCORE_ENTRY -Wl,-O1 -lm ../src/libs/win/pdcurses.a
DEPS = ../src/includes/*.h
STRIP_BINARY=yes
larn: $(DEPS) ../src/*.c
	$(CC) ../src/*.c -o larn13_win32 $(LIBS)
