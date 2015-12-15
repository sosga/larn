CC=gcc
LIBS= -Os -pipe -Wall -fomit-frame-pointer -DLINUX -Wl,-O1 -lm -lncurses
DEPS=*.h
STRIP_BINARY=yes
larn: $(DEPS) *.c
	$(CC) *.c -o larn1250 $(LIBS)
