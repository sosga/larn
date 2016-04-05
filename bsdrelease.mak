CC=gcc
LIBS= -Os -pipe -Wall -fomit-frame-pointer -DBSD -DMULTIPLE_SCORE_ENTRY -D_POSIX_C_SOURCE -lm -lncurses
DEPS=*.h
STRIP_BINARY=yes
larn: $(DEPS) *.c
	$(CC) *.c -o larn13_bsd $(LIBS)
