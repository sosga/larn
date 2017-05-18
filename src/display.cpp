#include <curses.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>

#include "../includes/action.h"
#include "config/larncons.h"
#include "config/data.h"
#include "config/larnfunc.h"
#include "terminal/term.hpp"
#include "../includes/create.h"
#include "../includes/display.h"
#include "../includes/global.h"
#include "../includes/io.h"
#include "../includes/monster.h"

using std::cout;

#define botsub( _idx, _x, _y, _str )        \
	if ( cdesc[(_idx)] != cbak[(_idx)] )        \
	{                                   \
		cbak[(_idx)] = cdesc[(_idx)];           \
		cursor( (_x), (_y) );               \
		lprintf( (_str), (int)cdesc[(_idx)] ); \
	}

#define nlprc(_ch) lprc(_ch)

static void bot_hpx(void);
static void bot_spellx(void);
static void botside(void);
static void seepage(void);
static int minx, maxx, miny, maxy;
static int bot1f = 0, bot2f = 0, bot3f = 0;
static int always = 0;
int regen_bottom = 0;

/*
bottomline()
now for the bottom line of the display
*/
void
bottomline(void)
{
    recalc();
    bot1f = 1;
}

void
bottomhp(void)
{
    bot2f = 1;
}

void
bottomspell(void)
{
    bot3f = 1;
}

void
bottomdo(void)
{
    if(bot1f)
        {
            bot3f = bot1f = bot2f = 0;
            bot_linex();
            return;
        }

    if(bot2f)
        {
            bot2f = 0;
            bot_hpx();
        }

    if(bot3f)
        {
            bot3f = 0;
            bot_spellx();
        }
}

void
bot_linex(void)
{
    int i;
    /*int debugtmp; */
    if(regen_bottom || (always))
        {
            regen_bottom = TRUE;

            if(cdesc[SPELLMAX] > 99)
                {
                    cursor(52, 1);
                    lprintf("Spells:");
                    cursor(59, 1);
                    lprintf("%3d(%3d)", (int) cdesc[SPELLS],
                            (int) cdesc[SPELLMAX]);
                }

            else
                {
                    cursor(52, 1);
                    lprintf("Spells:");
                    cursor(59, 1);
                    lprintf("%3d(%2d) ", (int) cdesc[SPELLS],
                            (int) cdesc[SPELLMAX]);
                }
            cursor(52, 2);
            lprintf("Level:");

            if(cdesc[LEVEL] > 99)
                {
                    cursor(59, 2);
                    lprintf("%3d", (int) cdesc[LEVEL]);
                }

            else
                {
                    cursor(59, 2);
                    lprintf("%-2d", (int) cdesc[LEVEL]);
                }
            cursor(52, 3);
            lprintf("HP:");
            cursor(56, 3);
            lprintf("%3d(%3d)", (int) cdesc[HP], (int) cdesc[HPMAX]);
            cursor(52, 4);
            lprintf("STR:");
            cursor(57, 4);
            lprintf("%-2d", (int)(cdesc[STRENGTH] + cdesc[STREXTRA]));
            cursor(62, 4);
            lprintf("INT:");
            cursor(67, 4);
            lprintf("%-2d", (int) cdesc[INTELLIGENCE]);
            cursor(72, 4);
            lprintf("WIS:");
            cursor(77, 4);
            lprintf("%-2d", (int) cdesc[WISDOM]);
            cursor(52, 5);
            lprintf("CON:");
            cursor(57, 5);
            lprintf("%-2d", (int) cdesc[CONSTITUTION]);
            cursor(62, 5);
            lprintf("DEX:");
            cursor(67, 5);
            lprintf("%-2d", (int) cdesc[DEXTERITY]);
            cursor(72, 5);
            lprintf("CHA:");
            cursor(77, 5);
            lprintf("%-2d", (int) cdesc[CHARISMA]);
            cursor(52, 6);
            lprintf("ACC:");
            cursor(57, 6);
            lprintf("%-3d", (int) cdesc[AC]);
            cursor(62, 6);
            lprintf("WCL:");
            cursor(67, 6);
            lprintf("%-3d", (int) cdesc[WCLASS]);
            cursor(52, 7);
            lprintf("Cave Level:");

            if((level == 0) || (wizard))
                {
                    cdesc[TELEFLAG] = 0;
                }

            if(cdesc[TELEFLAG])
                {
                    lprcat(" ?");
                }

            else
                {
                    lprcat(levelname[level]);
                }
            cursor(52, 8);
            lprintf("Gold:");
            cursor(58, 8);
            lprintf("%-6d", (int) cdesc[GOLD]);
            cursor(52, 9);
            lprintf("Exp:");
            cursor(57, 9);
            lprintf("%-9d",(int)cdesc[EXPERIENCE]);
            cursor(57, 10);
            lprintf("%s",classname[cdesc[LEVEL] - 1]);
            always = 1;
            botside();
            cdesc[TMP] = cdesc[STRENGTH] + cdesc[STREXTRA];

            for(i = 0; i < 100; i++)
                {
                    cbak[i] = cdesc[i];
                }

            return;
        }
    botsub(SPELLS, 59, 1, "%3d");

    if(cdesc[SPELLMAX] > 99)
        {
            botsub(SPELLMAX, 59, 1, "%3d)");
        }

    else
        {
            botsub(SPELLMAX, 59, 1, "%2d) ");
        }
    botsub(HP, 56, 3, "%3d");
    botsub(HPMAX, 61, 3, "%3d");
    botsub(AC, 57, 6, "%-3d");
    botsub(WCLASS, 67, 6, "%-3d");
    botsub(EXPERIENCE, 57, 9, "%-9d");

    if(cdesc[LEVEL] != cbak[LEVEL])
        {
            cursor(57, 10);
            lprcat(classname[cdesc[LEVEL] - 1]);
        }

    if(cdesc[LEVEL] > 99)
        {
            botsub(LEVEL, 57, 10, "%3d");
        }

    else
        {
            botsub(LEVEL, 57, 10, " %-2d");
        }

    cdesc[TMP] = cdesc[STRENGTH] + cdesc[STREXTRA];
    botsub(TMP, 57, 4, "%-2d");
    botsub(INTELLIGENCE, 67, 4, "%-2d");
    botsub(WISDOM, 77, 4, "%-2d");
    botsub(CONSTITUTION, 57, 5, "%-2d");
    botsub(DEXTERITY, 67, 5, "%-2d");
    botsub(CHARISMA, 77, 5, "%-2d");

    if((level != cbak[CAVELEVEL])
            || (cdesc[TELEFLAG] != cbak[TELEFLAG]))
        {
            if((level == 0) || (wizard))
                {
                    cdesc[TELEFLAG] = 0;
                }

            cbak[TELEFLAG] = cdesc[TELEFLAG];
            cbak[CAVELEVEL] = level;
            cursor(52, 7);
            if(cdesc[TELEFLAG])
                {
                    lprcat(" ?");
                }

            else
                {
                    lprcat(levelname[level]);
                }
        }

    botsub(GOLD, 58, 8, "%-6d");
    botside();
}

/*
special subroutine to update only the gold number on the bottomlines
called from ogold()
*/
void
bottomgold(void)
{
    botsub(GOLD, 58, 8, "%-6d");
}

/*
special routine to update hp and level fields on bottom lines
called in monster.c hitplayer() and spattack()
*/
static void
bot_hpx(void)
{
    /*This is added to update the color when hit.
     *It's redrawing over the top (1,19) ~Gibbon
     */
    if(cdesc[HP] < 10)
        {
            cursor(52, 3);
            lprintf("HP:");
            cursor(56, 3);
            lprintf("%3d", (int) cdesc[HP]);
        }

    else
        {
            cursor(52, 3);
            lprintf("HP:");
            cursor(56, 3);
            lprintf("%3d", (int) cdesc[HP]);
        }

    if(cdesc[EXPERIENCE] != cbak[EXPERIENCE])
        {
            recalc();
            bot_linex();
        }

    else
        {
            botsub(HP, 56, 3, "%3d");
        }
}

/*
special routine to update number of spells called from regen()
*/
static void
bot_spellx(void)
{
    botsub(SPELLS, 59, 1, "%2d");
}

/*
common subroutine for a more economical bottomline()
*/
struct bot_side_def
{

    int typ;
    char *string;

};

static struct bot_side_def bot_data[] =
{
    {STEALTH, (char*) "stealth"},
    {UNDEADPRO, (char*) "undead pro"},
    {SPIRITPRO, (char*) "spirit pro"},
    {CHARMCOUNT, (char*) "Charm"},
    {TIMESTOP, (char*) "Time Stop"},
    {HOLDMONST, (char*) "Hold Monst"},
    {GIANTSTR, (char*) "Giant Str"},
    {FIRERESISTANCE, (char*) "Fire Resit"},
    {DEXCOUNT, (char*) "Dexterity"},
    {STRCOUNT, (char*) "Strength"},
    {SCAREMONST, (char*) "Scare"},
    {HASTESELF, (char*) "Haste Self"},
    {CANCELLATION, (char*) "Cancel"},
    {INVISIBILITY, (char*) "Invisible"},
    {ALTPRO, (char*) "Protect 3"},
    {PROTECTIONTIME, (char*) "Protect 2"},
    {WTW, (char*) "Wall-Walk"}
};

static void
botside(void)
{
    int i, idx;

    for(i = 0; i < 17; i++)
        {
            idx = bot_data[i].typ;

            if((always) || (cdesc[idx] != cbak[idx]))
                {
                    if((always) || (cbak[idx] == 0))
                        {
                            if(cdesc[idx])
                                {
                                    cursor(52, i + 11);
                                    lprcat(bot_data[i].string);
                                    /*Reset cursor position. ~Gibbon */
                                    cursors();
                                }
                        }

                    else if(cdesc[idx] == 0)
                        {
                            cursor(52, i + 11);
                            lprcat("          ");
                            /*Reset cursor position. ~Gibbon */
                            cursors();
                        }

                    cbak[idx] = cdesc[idx];
                }
        }

    always = 0;
}

/*
*  subroutine to draw only a section of the screen
*  only the top section of the screen is updated.  If entire lines are being
*  drawn, then they will be cleared first.
*/
static int d_xmin = 0, d_xmax = MAXX, d_ymin = 0,
           d_ymax = MAXY;	/* for limited screen drawing */
void
draws(int xmin, int xmax, int ymin, int ymax)
{
    int i, idx;

    if(xmin == 0
            && xmax == MAXX)  	/* clear section of screen as needed */
        {
            if(ymin == 0)
                {
                    cl_up(79, ymax);
                }

            else
                for(i = ymin; i < ymin; i++)
                    {
                        cl_line(1, i + 1);
                    }

            xmin = -1;
        }

    d_xmin = xmin;
    d_xmax = xmax;
    d_ymin = ymin;
    d_ymax = ymax;		/* for limited screen drawing */
    drawscreen();

    if(xmin <= 0
            && xmax ==
            MAXX)  	/* draw stuff on right side of screen as needed */
        {
            for(i = ymin; i < ymax; i++)
                {
                    idx = bot_data[i].typ;

                    if(cdesc[idx])
                        {
                            cursor(52, i + 11);
                            lprcat(bot_data[i].string);
                        }

                    cbak[idx] = cdesc[idx];
                }
        }
}

/*
drawscreen()

subroutine to redraw the whole screen as the player knows it
*/
static int d_flag;
void
drawscreen(void)
{
    int i, j, k, ileft, iright;

    if(d_xmin == 0 && d_xmax == MAXX && d_ymin == 0
            && d_ymax == MAXY)
        {
            /* clear the screen */
            d_flag = 1;
            screen_clear();
        }

    else
        {
            d_flag = 0;
            cursor(1, 1);
        }

    if(d_xmin < 0)
        {
            /* d_xmin=-1 means display all without bottomline */
            d_xmin = 0;
        }

    /* display lines of the screen */
    for(j = d_ymin; j < d_ymax; j++)
        {
            /* When we show a spot of the dungeon, we have 4 cases:
               squares we know nothing about
               - know == 0
               squares we've been at and still know whats there
               - know == KNOWALL (== KNOWHERE | HAVESEEN)
               squares we've been at, but don't still recall because
               something else happened there.
               - know == HAVESEEN
               squares we recall, but haven't been at (an error condition)
               - know == KNOWHERE

               to minimize printing of spaces, scan from left of line until
               we reach a location that the user knows.
             */
            ileft = d_xmin - 1;

            while(++ileft < d_xmax)
                {
                    if(know[ileft][j])
                        {
                            break;
                        }
                }

            /* if blank line ... */
            if(ileft >= d_xmax)
                {
                    continue;
                }

            /* scan from right of line until we reach a location that the
               user knows.
             */
            iright = d_xmax;

            while(--iright > ileft)
                {
                    if(know[iright][j])
                        {
                            break;
                        }
                }

            /*
             * now print the line, after positioning the cursor.
             * print the line with bold objects in a different
             * loop for effeciency
             */
            cursor(ileft + 1, j + 1);

            for(i = ileft; i <= iright; i++)
                {
                    /* we still need to check for the location being known,
                       for we might have an unknown spot in the middle of
                       an otherwise known line.
                     */
                    if(know[i][j] == 0)
                        {
                            nlprc(' ');
                        }

                    else if(know[i][j] & HAVESEEN)
                        {
                            /*
                             * if monster there and the user still knows the place,
                             * then show the monster.  Otherwise, show what was
                             * there before.
                             */
                            if(i == playerx && j == playery)
                                {
                                    nlprc('@');
                                    continue;
                                }

                            k = mitem[i][j];

                            if(k && know[i][j] & KNOWHERE)
                                {
                                    nlprc(monstnamelist[k]);
                                }

                            else
                                {
                                    nlprc(objnamelist[item[i][j]]);
                                }
                        }

                    else
                        {
                            /*
                             * error condition.  recover by resetting location
                             * to an 'unknown' state.
                             */
                            nlprc(' ');
                            mitem[i][j] = item[i][j] = 0;
                        }
                }
        }

    resetbold();

    if(d_flag)
        {
            always = 1;
            botside();
            always = 1;
            bot_linex();
        }

    /* oldx=99; */
    /* for limited screen drawing */
    d_xmin = d_ymin = 0;
    d_xmax = MAXX;
    d_ymax = MAXY;
}

/*
showcell(x,y)

subroutine to display a cell location on the screen
*/
void
showcell(int x, int y)
{
    int i, j, k, m;

    if(cdesc[BLINDCOUNT])
        {
            return;  /* see nothing if blind     */
        }

    if(cdesc[AWARENESS])
        {
            minx = x - 3;
            maxx = x + 3;
            miny = y - 3;
            maxy = y + 3;
        }

    else
        {
            minx = x - 1;
            maxx = x + 1;
            miny = y - 1;
            maxy = y + 1;
        }

    if(minx < 0)
        {
            minx = 0;
        }

    if(maxx > MAXX - 1)
        {
            maxx = MAXX - 1;
        }

    if(miny < 0)
        {
            miny = 0;
        }

    if(maxy > MAXY - 1)
        {
            maxy = MAXY - 1;
        }

    for(j = miny; j <= maxy; j++)
        for(m = minx; m <= maxx; m++)
            if((know[m][j] & KNOWHERE) == 0)
                {
                    cursor(m + 1, j + 1);
                    x = maxx;

                    while(know[x][j] & KNOWHERE)
                        {
                            --x;
                        }

                    for(i = m; i <= x; i++)
                        {
                            if((k = mitem[i][j]) != 0)
                                {
                                    lprc(monstnamelist[k]);
                                }

                            else
                                switch(k = item[i][j])
                                    {
                                    case OWALL:
                                    case 0:
                                    case OIVTELETRAP:
                                    case OTRAPARROWIV:
                                    case OIVDARTRAP:
                                    case OIVTRAPDOOR:
                                        lprc(objnamelist[k]);
                                        break;

                                    default:
                                        lprc(objnamelist[k]);
                                        break;
                                    };

                            know[i][j] = KNOWALL;
                        }

                    m = maxx;
                }
}

/*
this routine shows only the spot that is given it.  the spaces around
these coordinated are not shown
used in godirect() in monster.c for missile weapons display
*/
void
show1cell(int x, int y)
{
    int k;
    cursor(x + 1, y + 1);

    /* see nothing if blind, but clear previous player position */
    if(cdesc[BLINDCOUNT])
        {
            if(x == oldx && y == oldy)
                {
                    lprc(' ');
                }

            return;
        }

    k = mitem[x][y];

    if(k)
        {
            lprc(monstnamelist[k]);
        }

    else
        {
            k = item[x][y];

            switch(k)
                {
                case OWALL:
                case 0:
                case OIVTELETRAP:
                case OTRAPARROWIV:
                case OIVDARTRAP:
                case OIVTRAPDOOR:
                    lprc(objnamelist[k]);
                    break;

                default:
                    lprc(objnamelist[k]);
                }
        }

    /* we end up knowing about it */
    know[x][y] = KNOWALL;
}

/*
showplayer()

subroutine to show where the player is on the screen
cursor values start from 1 up
*/
void
showplayer(void)
{
    show1cell(oldx, oldy);
    cursor(playerx + 1, playery + 1);
    lprc('@');
    cursor(playerx + 1, playery + 1);
    oldx = playerx;
    oldy = playery;
    refresh();
}

/*
moveplayer(dir)

subroutine to move the player from one room to another
returns 0 if can't move in that direction or hit a monster or on an object
else returns 1
nomove is set to 1 to stop the next move (inadvertent monsters hitting
players when walking into walls) if player walks off screen or into wall
*/
int diroffx[] = { 0, 0, 1, 0, -1, 1, -1, 1, -1 };
int diroffy[] = { 0, 1, 0, -1, 0, -1, -1, 1, 1 };
int
moveplayer(int dir)
/*  from = present room #  direction = [1-north]
[2-east] [3-south] [4-west] [5-northeast]
[6-northwest] [7-southeast] [8-southwest]
if direction=0, don't move--just show where he is */
{
    int k, m, i, j;

    if(cdesc[CONFUSE])
        if(cdesc[LEVEL] < rnd(30))
            {
                dir = rund(9);     /*if confused any dir */
            }

    k = playerx + diroffx[dir];
    m = playery + diroffy[dir];

    if(k < 0 || k >= MAXX || m < 0 || m >= MAXY)
        {
            nomove = 1;
            return (y_larn_rep = 0);
        }

    i = item[k][m];
    j = mitem[k][m];

    /* prevent the player from moving onto a wall, or a closed door,
       unless the character has Walk-Through-Walls.
     */
    if((i == OCLOSEDDOOR || i == OWALL) && cdesc[WTW] == 0)
        {
            nomove = 1;
            return (y_larn_rep = 0);
        }

    if(k == 33 && m == MAXY - 1 && level == 1)
        {
            newcavelevel(0);

            for(k = 0; k < MAXX; k++)
                for(m = 0; m < MAXY; m++)
                    if(item[k][m] == OENTRANCE)
                        {
                            playerx = k;
                            playery = m;
                            positionplayer();
                            drawscreen();
                            return (0);
                        }
        }

    /* hit a monster
     */
    if(j > 0)
        {
            hitmonster(k, m);
            return (y_larn_rep = 0);
        }

    /* check for the player ignoring an altar
     */
    if(item[playerx][playery] == OALTAR && !prayed)
        {
            cursors();
            lprcat("\nYou have ignored the altar!");
            act_ignore_altar();
        }

    prayed = 0;
    lastpx = playerx;
    lastpy = playery;
    playerx = k;
    playery = m;

    if(i && i != OTRAPARROWIV && i != OIVTELETRAP
            && i != OIVDARTRAP
            && i != OIVTRAPDOOR)
        {
            return (y_larn_rep = 0);
        }

    else
        {
            return (1);
        }
}

/*
*  function to show what magic items have been discovered thus far
*  enter with -1 for just spells, anything else will give scrolls & potions
*/
static int lincount, count;
void
seemagic(int arg)
{
    int i, j, k, number;
    int sort[SPNUM +
             1];		/* OK as long as SPNUM > MAXSCROLL,MAXPOTION */
    count = lincount = 0;

    /* count and sort the known spell codes
     */
    for(j = 0; j <= SPNUM; j++)
        {
            sort[j] = SPNUM;
        }

    for(number = i = 0; i < SPNUM; i++)
        if(spelknow[i])
            {
                number++;
                j = 0;

                while(strncmp(spelcode[sort[j]], spelcode[i], 3) < 0)
                    {
                        j++;
                    }

                k = number - 1;

                while(k > j)
                    {
                        sort[k] = sort[k - 1], k--;
                    }

                sort[j] = i;
            }

    if(arg == -1)  	/* if display spells while casting one */
        {
            cl_up(79, ((number + 2) / 3 +
                       4));	/* lines needed for display */
            cursor(1, 1);
        }

    else
        {
            resetscroll();
            screen_clear();
        }

    lprcat("The magic spells you have discovered thus far:\n\n");

    for(i = 0; i < number; i++)
        {
            lprintf("%s %-20s ", spelcode[sort[i]], spelname[sort[i]]);
            seepage();
        }

    if(arg == -1)
        {
            seepage();
            more(FALSE);
            draws(0, MAXX, 0, ((number + 2) / 3 + 4));
            return;
        }

    lincount += 3;

    if(count != 0)
        {
            count = 2;
            seepage();
        }

    /* count and sort the known scrolls
     */
    for(j = 0; j <= MAXSCROLL; j++)
        {
            sort[j] = MAXSCROLL;
        }

    for(number = i = 0; i < MAXSCROLL; i++)
        if(scrollname[i][0])
            {
                number++;
                j = 0;

                while(strcmp(&scrollname[sort[j]][1],
                             &scrollname[i][1]) < 0)
                    {
                        j++;
                    }

                k = number - 1;

                while(k > j)
                    {
                        sort[k] = sort[k - 1], k--;
                    }

                sort[j] = i;
            }

    lprcat("\nThe magic scrolls you have found to date are:\n\n");
    count = 0;

    for(i = 0; i < number; i++)
        {
            lprintf("%-26s", &scrollname[sort[i]][1]);
            seepage();
        }

    lincount += 3;

    if(count != 0)
        {
            count = 2;
            seepage();
        }

    /* count and sort the known potions
     */
    for(j = 0; j <= MAXPOTION; j++)
        {
            sort[j] = MAXPOTION;
        }

    for(number = i = 0; i < MAXPOTION; i++)
        if(potionname[i][0])
            {
                number++;
                j = 0;

                while(strcmp(&potionname[sort[j]][1],
                             &potionname[i][1]) < 0)
                    {
                        j++;
                    }

                k = number - 1;

                while(k > j)
                    {
                        sort[k] = sort[k - 1], k--;
                    }

                sort[j] = i;
            }

    lprcat("\nThe magic potions you have found to date are:\n\n");
    count = 0;

    for(i = 0; i < number; i++)
        {
            lprintf("%-26s", &potionname[sort[i]][1]);
            seepage();
        }

    if(lincount != 0)
        {
            more(FALSE);
        }

    setscroll();
    drawscreen();
}

/*
*  subroutine to paginate the seemagic function
*/
static void
seepage(void)
{
    if(++count == 3)
        {
            lincount++;
            count = 0;
            lprc('\n');

            if(lincount > 17)
                {
                    lincount = 0;
                    more(FALSE);
                    screen_clear();
                }
        }
}
