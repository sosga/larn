

/*
*
* macros
*
*/

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
#define clear() (*lpnt++ =CLEAR, regen_bottom=TRUE)

/* macro to clear to end of line */
#define cltoeoln() (*lpnt++ = CL_LINE)

/* macro to seed the random number generator */
#define srand(x) (lrandx=x)

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
#define _creat creat
#define _read read
#define _open open
#define _lseek lseek
#define _close close
#define _write write
#define _chmod chmod
#define _getch ansiterm_getch
#define _S_IWRITE 0666
#endif
