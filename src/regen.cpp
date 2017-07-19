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

#include <curses.h>
#include <cstdlib>

#include "config/larncons.h"
#include "config/data.h"
#include "templates/math.t.hpp"
#include "../includes/display.h"
#include "../includes/global.h"
#include "../includes/inventory.h"
#include "../includes/io.h"
#include "../includes/regen.h"

/*
* regen()
*
* subroutine to regenerate player hp and spells
*
*/
void
regen(void)
{
    int i, flag;
    long *larn_cdesc_id;
    larn_cdesc_id = cdesc;
    larn_cdesc_id[MOVESMADE]++;

    if (larn_cdesc_id[TIMESTOP]) {
        if (--larn_cdesc_id[TIMESTOP] <= 0) {
            bottomline();
        }
        return;
    }				/* for stop time spell */

    flag = 0;

    if (larn_cdesc_id[STRENGTH] < 3) {
        larn_cdesc_id[STRENGTH] = 3;
        flag = 1;
    }

    if (larn_cdesc_id[HP] != larn_cdesc_id[HPMAX])

        if (larn_cdesc_id[REGENCOUNTER]-- <= 0) {	/* regenerate hit points */
            larn_cdesc_id[REGENCOUNTER] = 20 + larn_cdesc_id[LEVEL];

            if ((larn_cdesc_id[HP] += larn_cdesc_id[REGEN]) > larn_cdesc_id[HPMAX]) {
                larn_cdesc_id[HP] = larn_cdesc_id[HPMAX];
            }
            bottomhp();
        }
    /* regenerate spells */
    if (larn_cdesc_id[SPELLS] < larn_cdesc_id[SPELLMAX])
        if (larn_cdesc_id[ECOUNTER]-- <= 0) {
            larn_cdesc_id[ECOUNTER] = 100 + 4 * larn_cdesc_id[LEVEL] - larn_cdesc_id[ENERGY];
            larn_cdesc_id[SPELLS]++;
            bottomspell();
        }
    if (larn_cdesc_id[HERO])
        if (--larn_cdesc_id[HERO] <= 0 ) {
            for (i = 0; i < 5; i++) {
                larn_cdesc_id[i] -= 10;
            }
            flag = 1;
        }
    if (larn_cdesc_id[ALTPRO])
        if (--larn_cdesc_id[ALTPRO] <= 0) {
            larn_cdesc_id[MOREDEFENSES] -= 3;
            flag = 1;
        }
    if (larn_cdesc_id[PROTECTIONTIME])
        if (--larn_cdesc_id[PROTECTIONTIME] <= 0) {
            larn_cdesc_id[MOREDEFENSES] -= 2;
            flag = 1;
        }
    if (larn_cdesc_id[DEXCOUNT])
        if (--larn_cdesc_id[DEXCOUNT] <= 0) {
            larn_cdesc_id[DEXTERITY] -= 3;
            flag = 1;
        }
    if (larn_cdesc_id[STRCOUNT])
        if (--larn_cdesc_id[STRCOUNT] <= 0) {
            larn_cdesc_id[STREXTRA] -= 3;
            flag = 1;
        }
    if (larn_cdesc_id[BLINDCOUNT])
        if (--larn_cdesc_id[BLINDCOUNT] <= 0) {
            cursors();
            fl_display_message("\nThe blindness subsides");
        }
    if (larn_cdesc_id[CONFUSE])
        if (--larn_cdesc_id[CONFUSE] <= 0) {
            cursors();
            fl_display_message("\nYou regain your senses");
        }
    if (larn_cdesc_id[GIANTSTR])
        if (--larn_cdesc_id[GIANTSTR] <= 0) {
            larn_cdesc_id[STREXTRA] -= 20;
            flag = 1;
        }
    if (larn_cdesc_id[CHARMCOUNT])
        if ((--larn_cdesc_id[CHARMCOUNT]) <= 0) {
            flag = 1;
        }
    if (larn_cdesc_id[INVISIBILITY])
        if ((--larn_cdesc_id[INVISIBILITY]) <= 0) {
            flag = 1;
        }
    if (larn_cdesc_id[CANCELLATION])
        if ((--larn_cdesc_id[CANCELLATION]) <= 0) {
            flag = 1;
        }
    if (larn_cdesc_id[WTW])
        if ((--larn_cdesc_id[WTW]) <= 0) {
            flag = 1;
        }
    if (larn_cdesc_id[HASTESELF])
        if ((--larn_cdesc_id[HASTESELF]) <= 0) {
            flag = 1;
        }
    if (larn_cdesc_id[AGGRAVATE]) {
        --larn_cdesc_id[AGGRAVATE];
    }
    if (larn_cdesc_id[SCAREMONST])
        if ((--larn_cdesc_id[SCAREMONST]) <= 0) {
            flag = 1;
        }
    if (larn_cdesc_id[STEALTH])
        if ((--larn_cdesc_id[STEALTH]) <= 0) {
            flag = 1;
        }
    if (larn_cdesc_id[AWARENESS]) {
        --larn_cdesc_id[AWARENESS];
    }
    if (larn_cdesc_id[HOLDMONST])
        if ((--larn_cdesc_id[HOLDMONST]) <= 0) {
            flag = 1;
        }
    if (larn_cdesc_id[HASTEMONST]) {
        --larn_cdesc_id[HASTEMONST];
    }
    if (larn_cdesc_id[FIRERESISTANCE])
        if ((--larn_cdesc_id[FIRERESISTANCE]) <= 0) {
            flag = 1;
        }
    if (larn_cdesc_id[GLOBE])
        if (--larn_cdesc_id[GLOBE] <= 0) {
            larn_cdesc_id[MOREDEFENSES] -= 10;
            flag = 1;
        }
    if (larn_cdesc_id[SPIRITPRO])
        if (--larn_cdesc_id[SPIRITPRO] <= 0) {
            flag = 1;
        }
    if (larn_cdesc_id[UNDEADPRO])
        if (--larn_cdesc_id[UNDEADPRO] <= 0) {
            flag = 1;
        }
    if (larn_cdesc_id[HALFDAM])
        if ( --larn_cdesc_id[HALFDAM] <= 0) {
            cursors();
            fl_display_message("\nYou're feeling better");
        }
    if (larn_cdesc_id[SEEINVISIBLE])
        if (--larn_cdesc_id[SEEINVISIBLE] <= 0) {
            monstnamelist[INVISIBLESTALKER] = floorc;

            if (!larn_cdesc_id[BLINDCOUNT]) {
                cursors();
                fl_display_message("\nYour vision returns to normal");
            }
        }
    if (larn_cdesc_id[ITCHING]) {
        if (larn_cdesc_id[ITCHING] > 1)
            if ((larn_cdesc_id[WEAR] != -1 ) || (larn_cdesc_id[SHIELD] != -1 ))
                if (TRnd(100) < 50) {
                    larn_cdesc_id[WEAR] = larn_cdesc_id[SHIELD] = -1;
                    cursors();
                    fl_display_message("\nThe unbareable itching forces you to remove your armor!");
                    recalc();
                    bottomline();
                }
        if (--larn_cdesc_id[ITCHING] <= 0) {
            cursors();
            fl_display_message("\nThe irritation subsides!");
        }
    }
    if (larn_cdesc_id[CLUMSINESS]) {
        if (larn_cdesc_id[WIELD] != -1)
            if (larn_cdesc_id[CLUMSINESS] > 1)
                if (item[playerx][playery] == 0)	/* only if nothing there */
                    if (TRnd(100) < 33) {		/* drop your weapon due to clumsiness */
                        drop_object((int)larn_cdesc_id[WIELD]);
                    }
        if (--larn_cdesc_id[CLUMSINESS] <= 0) {
            cursors();
            fl_display_message("\nYou feel less awkward!");
        }
    }
    if (flag) {
        bottomline();
    }
}
