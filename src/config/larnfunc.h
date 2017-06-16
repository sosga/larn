#include <cstdlib>

using namespace std;

/* Templates
 * I am very happy to get rid of these crappy macros!
 * ~Gibbon
 */
template<typename T>
T rnd(T x)
{
	return static_cast<T>(rand() % (x)) + 1;
}

template<typename T>
T rund(T x)
{
	return static_cast<T>(rand() % (x));
}

/* macro to create scroll #'s with probability of occurrence */
#define newscroll() (scprob[rund(81)])

/* macro to return a potion # created with probability of occurrence */
#define newpotion() (potprob[rund(41)])

/* macro to return the + points on created leather armor */
#define newleather() (nlpts[rund(cdesc[HARDGAME]?13:15)])

/* macro to return the + points on chain armor */
#define newchain() (nch[rund(10)])

/* macro to return + points on plate armor */
#define newplate() (nplt[rund(cdesc[HARDGAME]?4:12)])

/* macro to return + points on new daggers */
#define newdagger() (ndgg[rund(13)])

/* macro to return + points on new swords */
#define newsword() (nsw[rund(cdesc[HARDGAME]?6:13)])

/* macro to destroy object at present location */
#define forget() (item[playerx][playery]=know[playerx][playery]=0)

/* macro to wipe out a monster at a location */
#define disappear(x,y) (mitem[x][y]=know[x][y]=0)

/* macro to turn on bold display for the terminal */
#define setbold() (*lpnt++ = ST_START)

/* macro to turn off bold display for the terminal */
#define resetbold() (*lpnt++ = ST_END)

/* macro to setup the scrolling region for the terminal */
#define setscroll() enable_scroll=1

/* macro to clear the scrolling region for the terminal */
#define resetscroll() enable_scroll=0

/* macro to clear the screen and home the cursor */
#define screen_clear() (*lpnt++ =CLEAR, regen_bottom=TRUE)

/* macro to clear to end of line */
#define cltoeoln() (*lpnt++ = CL_LINE)

/* min/max */
#ifndef min
	#define min(x,y) (((x)>(y))?(y):(x))
#endif
#ifndef max
	#define max(x,y) (((x)>(y))?(x):(y))
#endif

/*
*
* function declarations
*
*/

#ifndef WINDOWS
	#define _getch ansiterm_getch
#endif
