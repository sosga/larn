#include <cctype>
#include <curses.h>
#include "putch.hpp"
#include "term.hpp"

void ansiterm_out(const char *, int);
inline const int ANSITERM_ESC = 27;
