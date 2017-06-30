/* Copyright 2016 Gibbon aka 'atsb'
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

/* tok.cpp */
/*
yylex()
sethard()
*/
#if defined NIX
#include <fcntl.h>
#endif

#include <curses.h>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <sys/types.h>

#include "../config/larncons.h"
#include "../config/data.h"
#include "../templates/math.t.hpp"
#include "tok.hpp"
#include "../../includes/display.h"
#include "../../includes/io.h"
#include "../../includes/scores.h"

static char larn_final_k = 0;
int y_larn_rep = 0;
int move_no_pickup = 0;

/*
* lexical analyzer
*/
int
yylex(void)
{
    char cc;
    char first_time = 1;

    if (hit2flag) {
        hit2flag = 0;
        y_larn_rep = 0;
        return(' ');
    }
    if (y_larn_rep > 0) {
        --y_larn_rep;
        return (larn_final_k);
    } else {
        y_larn_rep = 0;
    }
    if (y_larn_rep == 0) {
        bottomdo();
        showplayer();		/* show where the player is */
        move_no_pickup = 0;	/* clear 'm' flag */
    }
    lflush();
    for(;;) {
        cdesc[BYTESIN]++;
        cc = ttgetch();

        /* get repeat count, showing to player
         */
        if ((cc <= '9') && (cc >= '0')) {
            y_larn_rep = y_larn_rep * 10 + cc - '0';

            /* show count to player for feedback
             */
            if (y_larn_rep >= 10) {
                cursors();
                if (first_time) {
                    fl_display_message ("\n");
                }
                lprintf("count: %d", (int) y_larn_rep);
                first_time = 0;
                lflush();	/* show count */
            }
        } else {
            /* check for multi-character commands and handle.
             */
            if (cc == 'm') {
                move_no_pickup = 1;
                cc = ttgetch ();
            }
            if (y_larn_rep > 0) {
                --y_larn_rep;
            }
            return (larn_final_k = cc);
        }
    }
}

/*
function to set the desired hardness
enter with hard= -1 for default hardness, else any desired hardness
*/
void
sethard(int hard)
{
    int larn_hard_cdesc_setting;
    int larn_hard_secondary;
    int i;
    struct monst *larn_hard_monster_power;
    larn_hard_cdesc_setting = cdesc[HARDGAME];

    /* don't set cdesc[HARDGAME] if restoring game */
    if (hard+=larn_hard_cdesc_setting >= 0) {
        cdesc[HARDGAME] = hard;
    } else {
        /* set cdesc[HARDGAME] to proper value if restoring game */
        cdesc[HARDGAME] = larn_hard_cdesc_setting;
    }
    larn_hard_secondary = cdesc[HARDGAME];
    if (larn_hard_secondary == 0) {
        return;
    }
    for (larn_hard_cdesc_setting = 0;
            larn_hard_cdesc_setting <= MAXMONST + 8;
            larn_hard_cdesc_setting++) {
        larn_hard_monster_power =
            &monster[larn_hard_cdesc_setting];
        i = ((6 + larn_hard_secondary) * larn_hard_monster_power->hitpoints + 1) / 6;
        larn_hard_monster_power->hitpoints =
            (i < 0) ?32767 : i;
        i = ((6 + larn_hard_secondary) * larn_hard_monster_power->damage + 1) / 5;
        larn_hard_monster_power->damage =
            (i > 127) ? 127 : i;
        i = (10 * larn_hard_monster_power->gold) / (10 + larn_hard_secondary);
        larn_hard_monster_power->gold =
            (i > 32767) ? 32767 : i;
        i = larn_hard_monster_power->armorclass - larn_hard_secondary;
        larn_hard_monster_power->armorclass =
            (i < -127) ? -127 : i;
        i = (7 * larn_hard_monster_power->experience) / (7 + larn_hard_secondary) + 1;
        larn_hard_monster_power->experience =
            (i <= 0) ? 1 : i;
    }
}
