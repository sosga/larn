/* Copyright 2017 Gibbon aka 'atsb'
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <iostream>
#include <string>
#include "../config/larncons.h"
#include "../config/data.h"
#include "../config/larnfunc.h"
#include "../../includes/global.h"
#include "../../includes/io.h"
#include "../../includes/scores.h"
#include "../../includes/store.h"
#include "../../includes/sysdep.h"
#include "../../includes/create.h"
#include "save.hpp"
/*
 *  routine to save the present level into storage
 */

void
save(void)
{
    struct cel *pcel;
    int *pitem, *pknow, *pmitem;
    int *phitp, *piarg;
    struct cel *pecel;
    pcel = NULL;

    /* pointer to past end of this level's cells */
    pecel = pcel;

    pitem = item[0];
    piarg = iarg[0];
    pknow = know[0];
    pmitem = mitem[0];
    phitp = hitp[0];

    while(pcel < pecel)
        {

            pcel->mitem = *pmitem++;
            pcel->hitp = *phitp++;
            pcel->item = *pitem++;
            pcel->know = *pknow++;
            pcel->iarg = *piarg++;
            pcel++;
        }
}

/*
 * routine to restore a level from storage
 */
void
load(void)
{
    struct cel *pcel;
    int *pitem;
    int *pknow;
    int *pmitem;
    int *phitp, *piarg;
    struct cel *pecel;
    pcel = NULL;

    /* pointer to past end of this level's cells */
    pecel = pcel;

    pitem = item[0];
    piarg = iarg[0];
    pknow = know[0];
    pmitem = mitem[0];
    phitp = hitp[0];

    while(pcel < pecel)
        {

            *pmitem++ = pcel->mitem;
            *phitp++ = pcel->hitp;
            *pitem++ = pcel->item;
            *pknow++ = pcel->know;
            *piarg++ = pcel->iarg;
            pcel++;
        }
}

static time_t zzz = 0;

int
savegame(char *fname)
{
    int i, k;
    struct sphere *sp;
    time_t temptime;

    lflush();
    save();
    ointerest();
    if(lcreat(fname) < 0)
        {
            lcreat((char *) 0);
            lprintf("\nCan't open file <%s> to save game\n", fname);
            return (-1);
        }

    set_score_output();

    lwrite(logname, LOGNAMESIZE);
    lwrite((char *) beenhere, (sizeof(int) * (MAXLEVEL + MAXVLEVEL)));
    lwrite((char *) &cdesc[0], 100 * sizeof(long));
    lwrite((char *) &gtime, 1 * sizeof(long));

    lprint(level);
    lprint(playerx);
    lprint(playery);

    lwrite((char *) iven, 26 * sizeof(int));
    lwrite((char *) ivenarg, 26 * sizeof(int));

    for(k = 0; k < MAXSCROLL; k++)
        {

            lprc(scrollname[k][0]);
        }

    for(k = 0; k < MAXPOTION; k++)
        {

            lprc(potionname[k][0]);
        }

    lwrite((char *) spelknow, SPNUM * sizeof(int));
    lprint(wizard);
    lprint(rmst);		/*  random monster generation counter */

    for(i = 0; i < 90; i++)
        {

            lprint(dnd_item[i].qty);
        }

    lwrite((char *) course, 25 * sizeof(int));
    lprint(cheat);

    for(i = 0; i < MAXMONST; i++)
        lprint(monster[i].genocided);	/* genocide info */

    for(sp = spheres; sp; sp = sp->p)
        lwrite((char *) sp, sizeof(struct sphere));	/* save spheres of annihilation */

    time(&zzz);
    temptime = zzz - initialtime;
    lwrite((char *) &temptime, sizeof(time_t));

    lprint(VERSION);
    lprint(SUBVERSION);

    lwclose();

    lastmonst[0] = 0;
    setscroll();
    lcreat((char *) 0);
    return (0);
}

void
restoregame(char *fname)
{
    int i, k;
    struct sphere *sp, *sp2;
    /*struct stat filetimes; */
    time_t temptime;

    cursors();
    lprcat("\nRestoring . . .");
    lflush();
    if(lopen(fname) <= 0)
        {
            lcreat((char *) 0);
            lprintf("\nCan't open file <%s>to restore game\n", fname);
            nap(NAPTIME);
            cdesc[GOLD] = cdesc[BANKACCOUNT] = 0;
            died(-265);
            return;
        }

    lrfill(logname, LOGNAMESIZE);
    lrfill((char *) beenhere, (sizeof(int) * (MAXLEVEL + MAXVLEVEL)));
    lrfill((char *) &cdesc[0], 100 * sizeof(long));
    lrfill((char *) &gtime, 1 * sizeof(long));

    level = cdesc[CAVELEVEL] = larint();
    playerx = larint();
    playery = larint();
    newcavelevel(level);

    lrfill((char *) iven, 26 * sizeof(int));
    lrfill((char *) ivenarg, 26 * sizeof(int));

    for(k = 0; k < MAXSCROLL; k++)
        {

            scrollname[k][0] = lgetc();
        }

    for(k = 0; k < MAXPOTION; k++)
        {

            potionname[k][0] = lgetc();
        }

    lrfill((char *) spelknow, SPNUM * sizeof(int));
    wizard = larint();
    rmst = larint();		/*  random monster creation flag */

    for(i = 0; i < 90; i++)
        {

            dnd_item[i].qty = larint();
        }

    lrfill((char *) course, 25 * sizeof(int));
    cheat = larint();


    for(i = 0; i < MAXMONST; i++)
        monster[i].genocided = larint();	/* genocide info */

    for(sp = 0, i = 0; i < cdesc[SPHCAST]; i++)
        {
            sp2 = sp;
            sp = (struct sphere *) operator new(sizeof(struct sphere));
            if(sp == 0)
                {
                    fprintf(stderr, "Can't malloc() for sphere space\n");
                    break;
                }
            lrfill((char *) sp, sizeof(struct sphere));	/* get spheres of annihilation */
            sp->p = 0;		/* null out pointer */
            if(i == 0)
                spheres = sp;		/* beginning of list */
            else
                sp2->p = sp;
        }

    time(&zzz);
    lrfill((char *) &temptime, sizeof(time_t));
    initialtime = zzz - temptime;

    if(wizard)
        {
            lrclose();
            cheat = 1;
            cursor(1, 23);
            lprcat("Wizard's are not allowed to load a saved game!\n");
            lflush();
            nap(NAPTIME);
            cdesc[GOLD] = cdesc[BANKACCOUNT] = 0;
            died(-266);
            return;
        }

    if(VERSION != larint() || SUBVERSION != larint())
        {
            lrclose();
            cheat = 1;
            cursor(1, 23);
            lprcat("Sorry, But your save file is for an older version of larn\n");
            lflush();
            nap(NAPTIME);
            cdesc[GOLD] = cdesc[BANKACCOUNT] = 0;
            died(-266);
            return;
        }

    lrclose();

    oldx = oldy = 0;

    /* died a post mortem death */
    if(cdesc[HP] < 0)
        {
            died(284);
            return;
        }

    /* if patch up lev 25 player */
    if(cdesc[LEVEL] == 25 && cdesc[EXPERIENCE] > skill[24])
        {
            long tmp;
            tmp = cdesc[EXPERIENCE] - skill[24];	/* amount to go up */
            cdesc[EXPERIENCE] = skill[24];
            raiseexperience(tmp);
        }
    load();
    gtime -= 1;			/* HACK for time advancing either on save or reload */
    lasttime = gtime - 1;
}