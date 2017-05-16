#include <cctype>
#include <curses.h>
#include "putch.hpp"
#include "term.hpp"

#define ANSITERM_ESC	27
void ansiterm_out ( const char *, int );
