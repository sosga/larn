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

#include "config/larncons.h"
#include "config/data.h"
#include "templates/math.t.hpp"
#include "../includes/display.h"
#include "../includes/global.h"
#include "../includes/inventory.h"
#include "../includes/io.h"
#include <curses.h>

static void fl_clear_for_inventory(void);
static void fl_draw_after_inventory(void);
static int qshowstr(char);

/* declare the player's inventory.  These should only be referenced
in this module.
iven     - objects in the player's inventory
ivenarg  - attribute of each item ( + values, etc )
ivensort - sorted inventory (so we don't sort each time)
*/
int iven[MAXINVEN];
int ivenarg[MAXINVEN];
int ivensort[MAXINVEN + 1];	/* extra is for sentinel */

static char srcount = 0;	/* line counter for showstr() */

/*
* Initialize the player's inventory
*/
void
fl_init_inventory ( void )
{
    int i;

    for ( i = 0; i < MAXINVEN; i++ ) {
        iven[i] = ivenarg[i] = 0;
        ivensort[i] = END_SENTINEL;
    }
    ivensort[MAXINVEN] = END_SENTINEL;
    if ( cdesc[HARDGAME] <= 0 ) {
        iven[0] = OLEATHER;
        iven[1] = ODAGGER;
        ivenarg[0] = ivenarg[1] = cdesc[WEAR] = ivensort[0] = 0;
        ivensort[1] = cdesc[WIELD] = 1;
    }
}

/*
* show character's inventory
*/
int
showstr(char select_allowed)
{
    int i, number, item_select;
    number = 3;
    for (i = 0; i < MAXINVEN; i++) {
        if (iven[i]) {
            number++;  /* count items in inventory */
        }
    }
    fl_clear_for_inventory();
    item_select = qshowstr (select_allowed);
    fl_draw_after_inventory();
    return item_select;
}

static int
qshowstr(char select_allowed)
{
    int k, itemselect = 0;
    srcount = 0;

    if (cdesc[GOLD]) {
        lprintf(".) %d gold pieces",cdesc[GOLD]);
        srcount++;
    }
    for (k = 0; k < MAXINVEN; k++)
        if (iven[k] != 0) {
            itemselect = fl_display_inventory(k,select_allowed);
        }
    lprintf("\nElapsed time is %d. You have %d mobuls left",gtime / 100,(TIMELIMIT() - gtime) / 100);
    itemselect = more(select_allowed);
    if (select_allowed) {
        return((itemselect > 0) ? itemselect : 0);
    } else {
        return(0);
    }
}

/*
* subroutine to clear screen depending on # lines to display
*/
static void
fl_clear_for_inventory(void)
{
    screen_clear();
    enable_scroll = 0;
}

/*
* subroutine to restore normal display screen depending on fl_clear_for_inventory()
*/
static void
fl_draw_after_inventory(void)
{
    enable_scroll = 1;
    drawscreen();
}

int
fl_display_inventory(int index, char select_allowed)
{
    int itemselect = 0;

    switch (iven[index]) {
    case OPOTION:
    case OSCROLL:
    case OLARNEYE:
    case OBOOK:
    case OSPIRITSCARAB:
    case ODIAMOND:
    case ORUBY:
    case OCUBEofUNDEAD:
    case OEMERALD:
    case OCHEST:
    case OCOOKIE:
    case OSAPPHIRE:
    case ONOTHEFT:
        lprc('\n');
        CLEAR_EOL();
        lprintf("%c) %s", index + 'a', objectname[iven[index]]);
        if (iven[index] == OPOTION && potionname[ivenarg[index]]) lprintf("%s", potionname[ivenarg[index]]);
        else if (iven[index] == OSCROLL && scrollname[ivenarg[index]]) lprintf("%s", scrollname[ivenarg[index]]);
        break;
    default:
        lprc('\n');
        CLEAR_EOL();
        lprintf("%c) %s", index + 'a', objectname[iven[index]]);
        if (ivenarg[index] > 0) lprintf(" +%d", ivenarg[index]);
        else if (ivenarg[index] < 0) lprintf( " %d", ivenarg[index]);
        break;
    }
    if (cdesc[WIELD] == index)
        fl_display_message(" (weapon in hand)");
    if ((cdesc[WEAR] == index) || (cdesc[SHIELD] == index))
        fl_display_message(" (being worn)");
    if (++srcount >= 23) {
        srcount = 0;
        itemselect = more(select_allowed);
        clear();
    }
    return itemselect;
}


/*
* function to put something in the players inventory
* returns 0 if success, 1 if a failure
*/
int
take ( int itm, int arg )
{
    int i, limit;

    /*  cursors(); */
    if ( ( limit = 15 + ( cdesc[LEVEL] >> 1 ) ) > MAXINVEN ) {
        limit = MAXINVEN;
    }

    for ( i = 0; i < limit; i++ )
        if ( iven[i] == 0 ) {
            iven[i] = itm;
            ivenarg[i] = arg;
            limit = 0;

            switch ( itm ) {
            case OPROTRING:
            case ODAMRING:
            case OBELT:
                limit = 1;
                break;

            case ODEXRING:
                cdesc[DEXTERITY] += ivenarg[i] + 1;
                limit = 1;
                break;

            case OSTRRING:
                cdesc[STREXTRA] += ivenarg[i] + 1;
                limit = 1;
                break;

            case OCLEVERRING:
                cdesc[INTELLIGENCE] += ivenarg[i] + 1;
                limit = 1;
                break;

            case OHAMMER:
                cdesc[DEXTERITY] += 10;
                cdesc[STREXTRA] += 10;
                cdesc[INTELLIGENCE] -= 10;
                limit = 1;
                break;

            case OORBOFDRAGON:
                cdesc[SLAYING]++;
                break;

            case OSPIRITSCARAB:
                cdesc[NEGATESPIRIT]++;
                break;

            case OCUBEofUNDEAD:
                cdesc[CUBEofUNDEAD]++;
                break;

            case ONOTHEFT:
                cdesc[NOTHEFT]++;
                break;

            case OSWORDofSLASHING:
                cdesc[DEXTERITY] += 5;
                limit = 1;
                break;
            };

            fl_display_message ( "\nYou pick up:" );

            fl_display_inventory ( i,0);

            if ( limit ) {
                bottomline ();
            }

            return ( 0 );
        }

    fl_display_message ( "\nYou can't carry anything else" );
    return ( 1 );
}



/*
* subroutine to drop an object  returns 1 if something there already else 0
*/
int
drop_object ( int k )
{
    int itm;

    if ( ( k < 0 ) || ( k >= MAXINVEN ) ) {
        return ( 0 );
    }

    itm = iven[k];
    cursors ();

    if ( itm == 0 ) {
        lprintf ( "\nYou don't have item %c! ", k + 'a' );
        return ( 1 );
    }

    if ( item[playerx][playery] ) {
        lprintf ( "\nThere's something here already: %s",
                  objectname[item[playerx][playery]] );
        dropflag = 1;
        return ( 1 );
    }

    if ( playery == MAXY - 1 && playerx == 33 ) {
        return ( 1 );  /* not in entrance */
    }

    item[playerx][playery] = itm;
    iarg[playerx][playery] = ivenarg[k];
    fl_display_message ( "\n  You drop:" );
    fl_display_inventory ( k,0);			/* show what item you dropped */
    know[playerx][playery] = 0;
    iven[k] = 0;

    if ( cdesc[WIELD] == k ) {
        cdesc[WIELD] = -1;
    }

    if ( cdesc[WEAR] == k ) {
        cdesc[WEAR] = -1;
    }

    if ( cdesc[SHIELD] == k ) {
        cdesc[SHIELD] = -1;
    }

    adjustcvalues ( itm, ivenarg[k] );
    dropflag =
        1;			/* say dropped an item so wont ask to pick it up right away */
    return ( 0 );
}
