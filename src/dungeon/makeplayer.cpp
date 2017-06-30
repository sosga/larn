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
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <setjmp.h>
#include "../config/larncons.h"
#include "../config/data.h"
#include "../templates/math.t.hpp"
#include "../terminal/term.hpp"
#include "../../includes/global.h"
#include "../../includes/io.h"
#include "../../includes/monster.h"
#include "../../includes/scores.h"
#include "../../includes/inventory.h"
#include "../save/save.hpp"
#include "dungeon.hpp"

/*
makeplayer()

subroutine to create the player and the players attributes
this is called at the beginning of a game and at no other time
*/
void
makeplayer(void)
{
    int i;
    scbr();
    screen_clear();
    /*  start player off with 15 hit points */
    cdesc[HPMAX] = cdesc[HP] = 25;
    /*  player starts at level one          */
    cdesc[LEVEL] = 1;
    /*  total # spells starts off as 3  */
    cdesc[SPELLMAX] = cdesc[SPELLS] = 1;
    /* start regeneration correctly */
    cdesc[REGENCOUNTER] = 16;
    cdesc[ECOUNTER] = 96;
    cdesc[SHIELD] = cdesc[WEAR] = cdesc[WIELD] = -1;

    for(i = 0; i < MAXINVEN; i++) {
        iven[i] = 0;
    }

    /* he knows protection, magic missile */
    spelknow[0] = spelknow[1] = 1;

    if(cdesc[HARDGAME] <= 0) {
        iven[0] = OLEATHER;
        iven[1] = ODAGGER;
        iven[2] = 0;
        ivenarg[1] = ivenarg[0] = cdesc[WEAR] = 0;
        cdesc[WIELD] = 1;
    }

    playerx = TRnd(MAXX - 2);
    playery = TRnd(MAXY - 2);
    regen_bottom = 1;

    /* make the attributes, ie str, int, etc. */
    for(i = 0; i < 6; i++) {
        cdesc[i] = MAXINVEN;
    }

    recalc();
    screen_clear();
    enter_name();
}