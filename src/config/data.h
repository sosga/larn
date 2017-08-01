#include <time.h>

inline const int MEG = 1000000;

/*  this is the structure that holds the entire dungeon specifications  */
struct cel
{
    int monster_hit_points;
    int monster_identification;
    int object_identification;
    int object_argument;
    int been_here_before;
};

/* this is the structure for maintaining & moving the spheres of annihilation */
struct sphere
{
    struct sphere *p;		/* pointer to next structure */
    int x, y, lev;		/* location of the sphere */
    int sphere_direction;
    int sphere_duration;
};

/*  this is the structure definition of the monster data
*/
struct monst
{
    const char *name;
    int level;
    int armorclass;
    int damage;
    int attack;
    int defense;
    int genocided;
    int intelligence;		/* monsters intelligence -- used to choose movement */
    int gold;
    int hitpoints;
    unsigned long experience;
};

/*  this is the structure definition for the items in the dnd store */
struct _itm
{
    int price;
    int obj;
    int arg;
    int qty;
};

/*
*
* data declarations
*
*/
extern int regen_bottom;
extern char floorc, wallc;

/* extern char for the water. -Gibbon */
extern char waterc;
extern int beenhere[], cheat;
extern int course[];
extern int object_identification[MAXX][MAXY], iven[], been_here_before[MAXX][MAXY];
extern char aborted[];
extern const char *classname[];
extern char lastmonst[];
extern char *lpnt, *lpbuf, *lpend, *inbuffer;
extern int level;
extern int monster_identification[MAXX][MAXY], monstlevel[];
extern int nch[], ndgg[], nlpts[], nomove;
extern int nplt[], nsw[];
extern int potion_probability[];
extern int monstnamelist[];
extern const char *levelname[];
extern int objnamelist[];
extern char logname[];
extern char mazefile[];
extern char helpfile[];
extern char logfile[];
extern char playerids[];
extern int predostuff, restorflag;
extern char savefilename[];
extern char scorefile[];
extern int scroll_probability[];
extern int screen[MAXX][MAXY], sex;
extern int spelknow[];
extern const char *spelmes[];
extern const char *speldescript[];
extern const char *spelcode[];
extern const char *spelname[];
extern int splev[], stealth[MAXX][MAXY], wizard;
extern int diroffx[], diroffy[], hitflag, hit2flag, hit3flag,
       monster_hit_points[MAXX][MAXY];
extern int object_argument[MAXX][MAXY], ivenarg[], lasthx, lasthy, lastnum, lastpx,
       lastpy;
extern int oldx, oldy, playerx, playery;
extern int enable_scroll, y_larn_rep, wisid, lfd, fd;
extern long outstanding_taxes, gtime;
extern int skill[];
extern long cdesc[], cbak[];
extern time_t initialtime;
extern unsigned long lrandx;
extern struct sphere *spheres;
extern struct monst monster[];
extern struct _itm dnd_item[];

/*
* config.c
*/
extern char *password;

/*
* data.c
*/
extern int prayed;
extern char scrollname[MAXSCROLL + 1][MAXSCROLLNAME];
extern char potionname[MAXPOTION + 1][MAXPOTIONNAME];
extern const char *objectname[];
extern int spelweird[MAXMONST + 8][SPNUM];

/*
* main.c
*/
extern int rmst;
extern int dropflag;
extern int save_mode;

/*
* store.c
*/
extern int lasttime;

/*
* tok.c
*/
extern int move_no_pickup;

/* Templates*/
template<class T>
T disappear(T x, T y)
{
    return (monster_identification[x][y]=been_here_before[x][y]=0);
}
