CC=gcc
LIBS= -Wall -Wextra -pedantic -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes -Wdeclaration-after-statement -Wshadow -Wmissing-declarations -Wold-style-definition -Wredundant-decls -g -D_FORTIFY_SOURCE=2 -DDEBUG -DWINDOWS -DMULTIPLE_SCORE_ENTRY -lm ../src/libs/win/pdcurses.a
DEPS=../src/includes/*.h
STRIP_BINARY=no
larn: $(DEPS) ../src/*.c
	$(CC) ../src/*.c -o ../larn13_win32_debug $(LIBS)
