CC=gcc
LIBS= -O -g -pipe -Wall -Wextra -ansi -std=gnu99 -pedantic -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes -Wdeclaration-after-statement -Wshadow -Wmissing-declarations -Wold-style-definition -Wredundant-decls -g -D_FORTIFY_SOURCE=2 -DDEBUG -DLINUX -DMULTIPLE_SCORE_ENTRY -lm -lncurses
DEPS=../src/includes/*.h
STRIP_BINARY=no
larn: $(DEPS) ../src/*.c
	$(CC) ../src/*.c -o larn13_gnulinux_debug $(LIBS)
