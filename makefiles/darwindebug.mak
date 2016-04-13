CC=gcc
LIBS= -O -pipe -Wall -Wextra -ansi -pedantic -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes -Wdeclaration-after-statement -Wshadow -Wmissing-declarations -Wold-style-definition -Wredundant-decls -g -D_FORTIFY_SOURCE=2 -DDEBUG -DDARWIN -DMULTIPLE_SCORE_ENTRY -lm -lncurses
DEPS=../src/includes/*.h
STRIP_BINARY=no
larn: $(DEPS) ../src/*.c
	$(CC) ../src/*.c -o ../larn13_osx_debug $(LIBS)
