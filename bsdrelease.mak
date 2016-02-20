CC=gcc
LIBS= -Os -pipe -Wall -fomit-frame-pointer -DBSD -DMULTIPLE_SCORE_ENTRY -lm -lncurses
DEPS=*.h
STRIP_BINARY=yes
larn: $(DEPS) *.c
	$(CC) *.c -o larn1252_bsd $(LIBS)
