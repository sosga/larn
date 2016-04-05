CC=gcc
LIBS= -Wall -Wextra -pedantic -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes -Wdeclaration-after-statement -Wshadow -Wmissing-declarations -Wold-style-definition -Wredundant-decls -g -D_FORTIFY_SOURCE=2 -DDEBUG -DWINDOWS -DMULTIPLE_SCORE_ENTRY -D_POSIX_C_SOURCE -lm win/pdcurses.a
DEPS=*.h
STRIP_BINARY=no
larn: $(DEPS) *.c
	$(CC) *.c -o larn13_win32_debug $(LIBS)
