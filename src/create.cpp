#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <setjmp.h>
#include "config/larncons.h"
#include "config/data.h"
#include "templates/math.t.hpp"
#include "terminal/term.hpp"
#include "../includes/global.h"
#include "../includes/io.h"
#include "../includes/monster.h"
#include "core/scores.hpp"
#include "save/save.hpp"
#include "mazes/maze.hpp"
#include "dungeon/dungeon.hpp"

static int cannedlevel(int);
static void treasureroom(int);
static void troom(int, int, int, int, int, int);
static void fillmroom(int, int, int);
static void froom(int, int, int);
static void fillroom(int, int);

/*
makemaze(level)
int level;
subroutine to make the caverns for a given level.  only walls are made.
*/

void
makemaze (int k)
{
    int i, j, tmp, tmp2, mx, mxl, mxh, my, myl, myh, z;

    if (k > 1 && (TRnd(17)<=4 || k==MAXLEVEL-1 || k==MAXLEVEL+MAXVLEVEL-1)) {
        /* read maze from data file */
        if (cannedlevel(k)) return;
    }

    if (k == 0) {
        tmp = 0;
    } else {
        tmp = OWALL;
    }

    for (i=0; i<MAXY; i++) for (j=0; j<MAXX; j++) object_identification[j][i] = tmp;
    if (k == 0) {
        return;
    }
    eat(1, 1);
    if (k == 1) object_identification[33][MAXY-1] = OENTRANCE;

    /*  now for open spaces -- not on level 10  */
    if (k != MAXLEVEL-1) {
        tmp2 = TRnd(3)+3;

        for (tmp=0; tmp<tmp2; tmp++) {
            my = TRnd(11)+2;
            myl = my-TRnd(2);
            myh = my+TRnd(2);

            if (k < MAXLEVEL) {
                mx = TRnd(44)+5;
                mxl = mx-TRnd(4);
                mxh = mx+TRnd(12)+3;
                z = 0;
            } else {
                mx = TRnd(60)+3;
                mxl = mx-TRnd(2);
                mxh = mx+TRnd(2);
                z = makemonst(k);
            }

            for (i=mxl; i<mxh; i++)
                for (j=myl; j<myh; j++) {
                    object_identification[i][j] = 0;
                    monster_identification[i][j] = z;
                    if (monster_identification[i][j] != 0) monster_hit_points[i][j] = monster[z].hitpoints;
                }
        }
    }

    if (k != MAXLEVEL-1) {
        my = TRnd(MAXY-2);
        for (i=1; i<MAXX-1; i++) {
            object_identification[i][my] = 0;
        }
    }
    if (k > 1) {
        treasureroom(k);
    }
}

/*
*  function to read in a maze from a const char array and perform a random iteration to choose one
*
*  Format of maze data file:  1st character = # of mazes in file (ascii digit)
*              For each maze: 18 lines (1st 17 used) 67 characters per line
*
*  Special characters in maze data file:
*
*      #   wall            D   door            .   random monster
*      ~   eye of larn     !   cure dianthroritis
*      -   random object
*/
static int
cannedlevel (int dungeon_level)
{
    int i, j, it, mit, arg, marg;
    const char *fl_larn_mazes = FL_Larn_Maze_Chars[TRund(FL_MAZES)];

    for (i=0; i<MAXY; i++)
        for (j=0; j<MAXX; j++) {
            it = mit = arg = marg = 0;

            switch (*fl_larn_mazes++) {
            case '#': /* wall */
                it = OWALL;
                break;

            case 'D': /* door */
                it = OCLOSEDDOOR;
                arg = TRnd(30);
                break;

            case '~': /* eye of larn */
                if (dungeon_level != MAXLEVEL-1) break;
                it = OLARNEYE;
                mit = TRund(8)+DEMONLORD;
                marg = monster[mit].hitpoints;
                break;

            case '!': /* cure dianthroritis */
                if (dungeon_level != MAXLEVEL + MAXVLEVEL-1) break;
                it = OPOTION;
                arg = 21;
                mit = DEMONLORD+7;
                marg = monster[mit].hitpoints;
                break;

            case '.': /* random monster */
                if (dungeon_level < MAXLEVEL) break;
                mit = makemonst(dungeon_level + 1);
                marg = monster[mit].hitpoints;
                break;

            case '-': /* random object */
                it = newobject(dungeon_level + 1, &arg);
                break;
            }
            object_identification[j][i] = it;
            object_argument[j][i] = arg;
            monster_identification[j][i] = mit;
            monster_hit_points[j][i] = marg;
            been_here_before[j][i] = 0;
        }
    return 1;
}

/*
*  function to make a treasure room on a level
*  level 10's treasure room has the eye in it and demon lords
*  level V3 has potion of cure dianthroritis and demon prince
*/
static void
treasureroom (int lv)
{
    int tx, ty, xsize, ysize;

    for (tx = 1 + TRnd(10); tx < MAXX - 10; tx += 10)
        if ((lv == MAXLEVEL-1) || (lv == MAXLEVEL+MAXVLEVEL-1) || TRnd(13) == 2) {
            xsize = TRnd(6) + 3;
            ysize = TRnd(3) + 3;
            ty = TRnd(MAXY-9) + 1; /* upper left corner of room */
            if (lv == MAXLEVEL - 1 || lv == MAXLEVEL+MAXVLEVEL - 1) {
                troom(lv, xsize, ysize, tx = tx + TRnd(MAXX - 24), ty, TRnd(3) + 6);
            } else {
                troom(lv, xsize, ysize, tx, ty, TRnd(9));
            }
        }
}

/*
*  subroutine to create a treasure room of any size at a given location
*  room is filled with objects and monsters
*  the coordinate given is that of the upper left corner of the room
*/
static void
troom (int lv, int xsize, int ysize, int tx, int ty, int glyph)
{
    int i, j;
    int tp1, tp2;

    for (j = ty - 1; j <= ty + ysize; j++)
        for (i = tx - 1; i <= tx + xsize; i++)	/* clear out space for room */
            object_identification[i][j] = 0;
    for (j = ty; j < ty + ysize; j++)
        for (i = tx; i < tx + xsize; i++) {	/* now put in the walls */
            object_identification[i][j] = OWALL;
            monster_identification[i][j] = 0;
        }
    for (j = ty + 1; j < ty + ysize - 1; j++)
        for (i = tx + 1; i < tx + xsize - 1; i++)	/* now clear out interior */
            object_identification[i][j] = 0;

    switch (TRnd (2)) {	/* locate the door on the treasure room */
    case 1:
        object_identification[i = tx + TRund (xsize)][j = ty + (ysize - 1) * TRund (2)] =
            OCLOSEDDOOR;
        object_argument[i][j] = glyph;	/* on horizontal walls */
        break;
    case 2:
        object_identification[i = tx + (xsize - 1) * TRund (2)][j = ty + TRund (ysize)] =
            OCLOSEDDOOR;
        object_argument[i][j] = glyph;	/* on vertical walls */
        break;
    };

    tp1 = playerx;
    tp2 = playery;
    playery = ty + (ysize >> 1);
        for (playerx = tx + 1; playerx <= tx + xsize - 2; playerx += 2) {
            for (i = 0, j = TRnd (6); i <= j; i++) {
                something (lv + 2);
                createmonster (makemonst (lv + 1));
            }
        }
            playerx = tp1;
            playery = tp2;
}

/*
* subroutine to create the objects in the maze for the given level
*/
void
makeobject(int j)
{
    int i;

    if(j == 0) {
        /*  entrance to dungeon */
        fillroom(OENTRANCE, 0);
        /*  the DND STORE */
        fillroom(ODNDSTORE, 0);
        fillroom(OSCHOOL, 0);	/*  college of Larn             */
        fillroom(OBANK, 0);	/*  1st national bank of larn   */
        fillroom(FL_OBJECT_TEMPLE_IN, 0);
        fillroom(OHOME, 0);	/*  the players home & family   */
        fillroom(OTRADEPOST, 0);	/*  the trading post            */
        fillroom(OLRS, 0);	/*  the larn revenue service    */
        return;
    }

    if(j == MAXLEVEL) {
        fillroom(FL_OBJECT_TEMPLE_OUT, 0);
    }

    /*  make the fixed objects in the maze STAIRS   */
    if((j > 0) && (j != MAXLEVEL - 1) && (j != MAXLEVEL + MAXVLEVEL - 1)) {
        fillroom(OSTAIRSDOWN, 0);
    }

    if((j > 1) && (j != MAXLEVEL)) {
        fillroom(OSTAIRSUP, 0);
    }

    /*  make the random objects in the maze     */
    fillmroom(TRund(3), OBOOK, j);
    fillmroom(TRund(3), OPRAYERBOOK, j);
    fillmroom(TRund(3), OALTAR, 0);
    fillmroom(TRund(3), OSTATUE, 0);
    fillmroom(TRund(3), OPIT, 0);
    fillmroom(TRund(3), OFOUNTAIN, 0);
    fillmroom(TRnd(3) - 2, OIVTELETRAP, 0);
    fillmroom(TRund(2), OTHRONE, 0);
    fillmroom(TRund(2), OMIRROR, 0);
    fillmroom(TRund(2), OTRAPARROWIV, 0);
    fillmroom(TRnd(3) - 2, OIVDARTRAP, 0);
    fillmroom(TRund(3), OCOOKIE, 0);

    if(j == 1) {
        fillmroom(1, OCHEST, j);
    }

    else {
        fillmroom(TRund(2), OCHEST, j);
    }

    if((j != MAXLEVEL - 1) && (j != MAXLEVEL + MAXVLEVEL - 1)) {
        fillmroom(TRund(2), OIVTRAPDOOR, 0);
    }

    if(j <= 10) {
        fillmroom((TRund(2)), ODIAMOND, TRnd(10 * j + 1) + 10);
        fillmroom(TRund(2), ORUBY, TRnd(6 * j + 1) + 6);
        fillmroom(TRund(2), OEMERALD, TRnd(4 * j + 1) + 4);
        fillmroom(TRund(2), OSAPPHIRE, TRnd(3 * j + 1) + 2);
    }

    for(i = 0; i < TRnd(4) + 3; i++) {
        fillroom(OPOTION, potion_probability[TRund(41)]);      /*  make a POTION   */
    }

    for(i = 0; i < TRnd(5) + 3; i++) {
        fillroom(OSCROLL, scroll_probability[TRund(81)]);      /*  make a SCROLL   */
    }

    for(i = 0; i < TRnd(12) + 11; i++) {
        fillroom(OGOLDPILE, 12 * TRnd(j + 1) + (j << 3) + 10);          /* make GOLD */
    }

    if(j == 5) {
        fillroom(OBANK2, 0);     /* branch office of the bank */
    }

    froom(2, ORING, 0);		/* a ring mail          */
    froom(1, OSTUDLEATHER, 0);	/* a studded leather    */
    froom(3, OSPLINT, 0);	/* a splint mail        */
    froom(5, OSHIELD, TRund(3));	/* a shield             */
    froom(2, OBATTLEAXE, TRund(3));	/* a battle axe         */
    froom(5, OLONGSWORD, TRund(3));	/* a long sword         */
    froom(4, OREGENRING, TRund(3));	/* ring of regeneration */
    froom(1, OPROTRING, TRund(3));	/* ring of protection   */
    froom(2, OSTRRING, 1 + TRnd(3));	/* ring of strength     */
    froom(7, OSPEAR, TRnd(5));	/* a spear              */
    froom(3, OORBOFDRAGON, 0);	/* orb of dragon slaying */
    froom(4, OSPIRITSCARAB, 0);	/* scarab of negate spirit */
    froom(4, OCUBEofUNDEAD, 0);	/* cube of undead control   */
    froom(2, ORINGOFEXTRA, 0);	/* ring of extra regen      */
    froom(3, ONOTHEFT, 0);	/* device of antitheft      */
    froom(2, OSWORDofSLASHING, 0);	/* sword of slashing */

    if(cdesc[BESSMANN] == 0) {
        froom(4, OHAMMER, 0);	/*Bessman's flailing hammer */
        cdesc[BESSMANN] = 1;
    }

    if((TRnd(4) == 3)) {
        if(j > 3) {
            froom(3, OSWORD, 3);	/* sunsword + 3         */
            froom(5, O2SWORD, TRnd(4));	/* a two handed sword */
            froom(5, OHSWORD, TRnd(4));	/* a longsword of Hymie     */
            froom(3, OBELT, 4);	/* belt of striking     */
            froom(3, OENERGYRING, 3);	/* energy ring          */
            froom(4, OPLATE, 5);	/* platemail + 5        */
            froom(3, OCLEVERRING, 1 + TRnd(2));	/* ring of cleverness */
        }
    }
}

/*
*  subroutine to fill in a number of objects of the same kind
*/
static void
fillmroom(int n, int what, int arg)
{
    int i;

    for(i = 0; i < n; i++) {
        fillroom(what, arg);
    }
}

static void
froom(int n, int itm, int arg)
{
    if(TRnd(151) < n) {
        fillroom(itm, arg);
    }
}

/*
* subroutine to put an object into an empty room
* uses a random walk
*/
static void
fillroom(int what, int arg)
{
    int x, y;
    cdesc[FILLROOM]++;
    x = TRnd(MAXX - 2);
    y = TRnd(MAXY - 2);

    while(object_identification[x][y]) {
        /* count up these random walks */
        cdesc[RANDOMWALK]++;
        x += TRnd(3) - 2;
        y += TRnd(3) - 2;

        /* clamp location to within map border */
        if(x > MAXX - 2) {
            x = 1;
        }

        if(x < 1) {
            x = MAXX - 2;
        }

        if(y > MAXY - 2) {
            y = 1;
        }

        if(y < 1) {
            y = MAXY - 2;
        }
    }

    object_identification[x][y] = what;
    object_argument[x][y] = arg;
}

/*
*  creates an entire set of monsters for a level
*  must be done when entering a new level
*  if sethp(1) then wipe out old monsters else leave them there
*/
void
sethp(int flg)
{
    int i, j;

    if(flg) {
        for(i = 0; i < MAXY; i++) {
            for(j = 0; j < MAXX; j++) {
                stealth[j][i] = 0;
            }
        }
    }

    /* if teleported and found level 1 then been_here_before level we are on */
    if(level == 0) {
        cdesc[TELEFLAG] = 0;
        return;
    }

    if(flg) {
        j = TRnd(12) + 2 + (level >> 1);
    }

    else {
        j = (level >> 1) + 1;
    }

    for(i = 0; i < j; i++) {
        fillmonst(makemonst(level));
    }
}

/*
*  Function to destroy all genocided monsters on the present level
*/
void
checkgen(void)
{
    int x,y;

    for (y=0; y<MAXY; y++)
        for (x=0; x<MAXX; x++)
            if(monster[monster_identification[x][y]].genocided)
                monster_identification[x][y]=0; /* no more monster */
}
