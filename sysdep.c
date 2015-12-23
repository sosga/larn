


/********************************************
 *                 WINDOWS                  *
 ********************************************/
#ifdef WINDOWS
#include <windows.h>
#include "larnfunc.h"


/*
 *  routine to take a nap for n milliseconds
 */
void nap(int x)
{

	/* eliminate chance for infinite loop */
	if (x <= 0) {

		return; 
	}
	
	lflush();
	
	Sleep(x);
}
#endif

#if defined LINUX || DARWIN || BSD

/********************************************
 *                 UNIX                     *
 ********************************************/

#include <unistd.h>
#include "larnfunc.h"


/*
 *  routine to take a nap for n milliseconds
 */
void nap(int x)
{

	/* eliminate chance for infinite loop */
	if (x <= 0) {

		return; 
	}
	
	lflush();
	
/* removed *1000, doing this simply makes it wait much longer */
	usleep(x);
}


#endif

