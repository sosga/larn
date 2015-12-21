CC=gcc
LIBS= -Os -pipe -Wall -fomit-frame-pointer -DDARWIN -lm -lncurses
DEPS=*.h
STRIP_BINARY=yes
larn: $(DEPS) *.c
	$(CC) *.c -o larn1251_osx $(LIBS)
