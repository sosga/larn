CC=gcc
LIBS= -Wall -Wextra -pedantic -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes -Wdeclaration-after-statement -Wshadow -Wmissing-declarations -Wold-style-definition -Wredundant-decls -g -D_FORTIFY_SOURCE=2 -DDEBUG -DWINDOWS -lm win/pdcurses.a win/panel.a 
DEPS=*.h
STRIP_BINARY=no
larn: $(DEPS) *.c
	$(CC) *.c -o larn1250_debug $(LIBS)
