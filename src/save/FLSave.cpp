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

#include <string>
#include "../../includes/main.h"
#include "../config/data.h"
#include "../templates/math.t.hpp"
#include "../../includes/global.h"
#include "../../includes/io.h"
#include "../core/scores.hpp"
#include "../core/store.hpp"
#include "../core/sysdep.hpp"
#include "../dungeon/dungeon.hpp"
#include "save.hpp"
#include "../core/funcs.hpp"
/*
 *  routine to save the present level into storage
 */

void
Save::save(void)
{
    struct cel *pcel;
    int *pitem, *pknow, *pmitem;
    int *phitp, *piarg;
    struct cel *pecel;
    pcel = NULL;

    /* pointer to past end of this level's cells */
    pecel = pcel;

    pitem = object_identification[0];
    piarg = object_argument[0];
    pknow = been_here_before[0];
    pmitem = monster_identification[0];
    phitp = monster_hit_points[0];

    while(pcel < pecel) {

        pcel->monster_identification = *pmitem++;
        pcel->monster_hit_points = *phitp++;
        pcel->object_identification = *pitem++;
        pcel->been_here_before = *pknow++;
        pcel->object_argument = *piarg++;
        pcel++;
    }
}

/*
 * routine to restore a level from storage
 */
void
Load::load(void)
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

    pitem = object_identification[0];
    piarg = object_argument[0];
    pknow = been_here_before[0];
    pmitem = monster_identification[0];
    phitp = monster_hit_points[0];

    while(pcel < pecel) {

        *pmitem++ = pcel->monster_identification;
        *phitp++ = pcel->monster_hit_points;
        *pitem++ = pcel->object_identification;
        *pknow++ = pcel->been_here_before;
        *piarg++ = pcel->object_argument;
        pcel++;
    }
}

static time_t zzz = 0;

int
Save::savegame(char *fname)
{
    int i, k;
    struct sphere *sp;
    time_t temptime;
    Save save;

    fl_output_buffer_flush();
    save.save();
    ointerest();
    if(lcreat(fname) < 0) {
        lcreat(reinterpret_cast<char *>(0));
        lprintf("\nCan't open file <%s> to save game\n", fname);
        return (-1);
    }

    set_score_output();

    lwrite(logname, LOGNAMESIZE);

    lwrite(reinterpret_cast<char *>(beenhere), (sizeof(int) * (MAXLEVEL + MAXVLEVEL)));
    lwrite(reinterpret_cast<char *>(&cdesc[0]), 100 * sizeof(long));
    lwrite(reinterpret_cast<char *>(&gtime), 1 * sizeof(long));

    lprint(level);
    lprint(playerx);
    lprint(playery);

    lwrite(reinterpret_cast<char *>(iven), 26 * sizeof(int));
    lwrite(reinterpret_cast<char *>(ivenarg), 26 * sizeof(int));

    for(k = 0; k < MAXSCROLL; k++) {

        lprc(scrollname[k][0]);
    }

    for(k = 0; k < MAXPOTION; k++) {

        lprc(potionname[k][0]);
    }

    lwrite(reinterpret_cast<char *>(spelknow), SPNUM * sizeof(int));
    lprint(wizard);
    lprint(rmst);		/*  random monster generation counter */

    for(i = 0; i < 90; i++) {

        lprint(dnd_item[i].qty);
    }

    lwrite(reinterpret_cast<char *>(course), 25 * sizeof(int));
    lprint(cheat);

    for(i = 0; i < MAXMONST; i++)
        lprint(monster[i].genocided);	/* genocide info */

    for(sp = spheres; sp; sp = sp->p)
        lwrite(reinterpret_cast<char *>(sp), sizeof(struct sphere));	/* save spheres of annihilation */

    time(&zzz);
    temptime = zzz - initialtime;
    lwrite(reinterpret_cast<char *>(&temptime), sizeof(time_t));

    lprintf("%s",VERSION);

    lwclose();

    lastmonst[0] = 0;
    enable_scroll = 1;
    lcreat(reinterpret_cast<char *>(0));
    return (0);
}

void
Load::restoregame(char *fname)
{
    FLCoreFuncs CoreFuncs;
    int i, k;
    struct sphere *sp, *sp2;
    /*struct stat filetimes; */
    time_t temptime;
    Load load;

    fl_termcap_cursor_position(1,24);
    fl_display_message("\nRestoring . . .");
    fl_output_buffer_flush();
    if(lopen(fname) <= 0) {
        lcreat(reinterpret_cast<char *>(0));
        lprintf("\nCan't open file <%s>to restore game\n", fname);
        fl_wait(FL_WAIT_DURATION);
        cdesc[GOLD] = cdesc[BANKACCOUNT] = 0;
        fl_player_death(-265);
        return;
    }

    lrfill(logname, LOGNAMESIZE);
    lrfill(reinterpret_cast<char *>(beenhere), (sizeof(int) * (MAXLEVEL + MAXVLEVEL)));
    lrfill(reinterpret_cast<char *>(&cdesc[0]), 100 * sizeof(long));
    lrfill(reinterpret_cast<char *>(&gtime), 1 * sizeof(long));

    level = cdesc[CAVELEVEL] = larint();
    playerx = larint();
    playery = larint();
    newcavelevel(level);

    lrfill(reinterpret_cast<char *>(iven), 26 * sizeof(int));
    lrfill(reinterpret_cast<char *>(ivenarg), 26 * sizeof(int));

    for(k = 0; k < MAXSCROLL; k++) {

        scrollname[k][0] = lgetc();
    }

    for(k = 0; k < MAXPOTION; k++) {

        potionname[k][0] = lgetc();
    }

    lrfill(reinterpret_cast<char *>(spelknow), SPNUM * sizeof(int));
    wizard = larint();
    rmst = larint();		/*  random monster creation flag */

    for(i = 0; i < 90; i++) {

        dnd_item[i].qty = larint();
    }

    lrfill(reinterpret_cast<char *>(course), 25 * sizeof(int));
    cheat = larint();


    for(i = 0; i < MAXMONST; i++)
        monster[i].genocided = larint();	/* genocide info */

    for(sp = 0, i = 0; i < cdesc[SPHCAST]; i++) {
        sp2 = sp;
        sp = (struct sphere *) operator new(sizeof(struct sphere));
        if(sp == 0) {
            fprintf(stderr, "Can't malloc() for sphere space\n");
            break;
        }
        lrfill(reinterpret_cast<char *>(sp), sizeof(struct sphere));	/* get spheres of annihilation */
        sp->p = 0;		/* null out pointer */
        if(i == 0)
            spheres = sp;		/* beginning of list */
        else
            sp2->p = sp;
    }

    time(&zzz);
    lrfill(reinterpret_cast<char *>(&temptime), sizeof(time_t));
    initialtime = zzz - temptime;

    lrclose();

    oldx = oldy = 0;

    /* died a post mortem death */
    if(cdesc[FL_HP] < 0) {
        fl_player_death(284);
        return;
    }

    /* if patch up lev 25 player */
    if(cdesc[FL_LEVEL] == 25 && cdesc[EXPERIENCE] > skill[24]) {
        long tmp;
        tmp = cdesc[EXPERIENCE] - skill[24];	/* amount to go up */
        cdesc[EXPERIENCE] = skill[24];
        CoreFuncs.IncreaseExperience(tmp);
    }

    load.load();
    gtime -= 1;			/* HACK for time advancing either on save or reload */
    lasttime = gtime - 1;
}
