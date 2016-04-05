CC=gcc
LIBS= -Os -pipe -Wall -fomit-frame-pointer -DDARWIN -DMULTIPLE_SCORE_ENTRY -lm -lncurses
DEPS=*.h
STRIP_BINARY=yes
larn: $(DEPS) *.c
	$(CC) *.c -o larn13_osx $(LIBS)
