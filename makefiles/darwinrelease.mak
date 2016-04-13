CC=gcc
LIBS= -Os -pipe -Wall -fomit-frame-pointer -DDARWIN -DMULTIPLE_SCORE_ENTRY -lm -lncurses
DEPS=../src/includes/*.h
STRIP_BINARY=yes
larn: $(DEPS) ../src/*.c
	$(CC) ../src/*.c -o ../larn13_osx $(LIBS)
