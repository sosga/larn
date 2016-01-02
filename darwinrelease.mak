CC=gcc
LIBS= -Os -pipe -Wall -fomit-frame-pointer -DDARWIN -lm -lncurses
DEPS=*.h
STRIP_BINARY=yes
larn: $(DEPS) *.c
	$(CC) *.c -o larn1252_osx $(LIBS)
