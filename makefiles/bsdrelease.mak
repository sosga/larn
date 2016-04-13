CC=gcc
LIBS= -Os -pipe -Wall -fomit-frame-pointer -ansi -std=gnu99 -DBSD -DMULTIPLE_SCORE_ENTRY -lm -lncurses
DEPS=../src/includes/*.h
STRIP_BINARY=yes
larn: $(DEPS) ../src/*.c
	$(CC) ../src/*.c -o larn13_bsd $(LIBS)
