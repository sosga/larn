CC=gcc
LIBS= -O -g -pipe -Wall -Wextra -ansi -pedantic -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes -Wdeclaration-after-statement -Wshadow -Wmissing-declarations -Wold-style-definition -Wredundant-decls -g -D_FORTIFY_SOURCE=2 -DDEBUG -DLINUX -DMULTIPLE_SCORE_ENTRY -lm -lncurses
DEPS=*.h
STRIP_BINARY=no
larn: $(DEPS) *.c
	$(CC) *.c -o larn1252_linux_debug $(LIBS)
