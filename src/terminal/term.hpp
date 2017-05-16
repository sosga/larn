#include <curses.h>
#include <string>

void ansiterm_init ( void );
void ansiterm_clean_up ( void );
void ansiterm_command ( int, const char *,const char * );

