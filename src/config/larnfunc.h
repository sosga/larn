#include <cstdlib>

using namespace std;

/* Templates
 * I am very happy to get rid of these crappy macros!
 * ~Gibbon
 */
template<typename T>
T TRnd(T x)
{
	return static_cast<T>(rand() % (x)) + 1;
}

template<typename T>
T TRund(T x)
{
	return static_cast<T>(rand() % (x));
}

template<class T>
T TMathMin(T x, T y)
{
	return x > y ? y : x;
}

template<class T>
T TMathMax(T x, T y)
{
	return x > y ? x : y;
}

/* macro to create scroll #'s with probability of occurrence */
#define NewScroll() (scprob[TRund(81)])

/* macro to return a potion # created with probability of occurrence */
#define newpotion() (potprob[TRund(41)])

/* macro to return the + points on created leather armor */
#define newleather() (nlpts[TRund(cdesc[HARDGAME]?13:15)])

/* macro to return the + points on chain armor */
#define newchain() (nch[TRund(10)])

/* macro to return + points on plate armor */
#define newplate() (nplt[TRund(cdesc[HARDGAME]?4:12)])

/* macro to return + points on new daggers */
#define newdagger() (ndgg[TRund(13)])

/* macro to return + points on new swords */
#define newsword() (nsw[TRund(cdesc[HARDGAME]?6:13)])

/* macro to destroy object at present location */
#define forget() (item[playerx][playery]=know[playerx][playery]=0)

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

/*
*
* function declarations
*
*/

#ifndef WINDOWS
	#define _getch ansiterm_getch
#endif
