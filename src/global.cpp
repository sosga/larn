/*  global.c
*
*  raiselevel()        subroutine to raise the player one level
*  loselevel()         subroutine to lower the player by one level
*  raiseexperience(x)  subroutine to increase experience points
*  loseexperience(x)   subroutine to lose experience points
*  DecreasePHealth(x)           subroutine to remove hit points from the player
*  FL_LOSEMAXHEALTH(x)          subroutine to remove max # hit points from the player
*  raisehp(x)          subroutine to gain hit points
*  FL_LOSEMAXSPELLS(x)      subroutine to lose maximum spells
*  makemonst(lev)      function to return monster number for a randomly selected monster
*  positionplayer()    function to be sure player is not in a wall
*  fl_recalculate_armor_class()            function to recalculate the armor class of the player
*  quit()              subroutine to ask if the player really wants to quit
*  more()
*  take()
*  drop_object()
*  enchantarmor()
*  enchweapon()
*  pocketfull()
*  nearbymonst()
*  stealsomething()
*  emptyhanded()
*  creategem()
*  adjustcvalues()
*  gettokstr()
*  getpassword()
*  getyn()
*  packweight()
*/

#include "core/inventory.hpp"
#include "core/scores.hpp"
#include "core/sysdep.hpp"
#include "core/funcs.hpp"
#include "config/data.h"
#include "templates/math.t.hpp"
#include "../includes/display.h"
#include "../includes/global.h"
#include "../includes/io.h"
#include "../includes/monster.h"
#include "../includes/main.h"

/*
 *   makemonst(lev)
 *       int lev;
 *
 * function to return monster number for a randomly selected monster
 * for the given cave level
 *
 * FIXED by ~Gibbon.  Randomness was removed prior to 12.4alpha2 for some reason, so
 * monsters were being taken based on the int of monstlevel (5,11 etc).
 *
 * Basically unchanged from 12.3 but cleaned up.  Don't fix what isn't broken.
 */
int
makemonst ( int lev )
{
    int x, tmp;

    if ( lev < 1 ) {
        lev = 1;
    }

    else if ( lev > 12 ) {
        lev = 12;
    }

    if ( lev < 5 ) {
        tmp = TRnd ( ( x = monstlevel[lev - 1] ) ? x : 1 );
    }

    else {
        tmp =
            TRnd ( ( x =
                         monstlevel[lev - 1] - monstlevel[lev - 4] ) ? x : 1 ) +
            monstlevel[lev - 4];
    }

    while ( monster[tmp].genocided && tmp < MAXMONST ) {
        tmp++;			/* genocided? */
    }

    return tmp;
}

/*
* positionplayer()
*
* Insure player is not in a wall or on top of a monster.  Could be more
* intelligent about what kinds of objects the player can land on.
*/
void
positionplayer ( void )
{
    /* The previous code was conflicting with changes I made
     * to maze generation and it caused a crash every time
     * LV10 was reached. ~Gibbon
     */
    int pp_try = 2;

    while ((object_identification[playerx][playery] || monster_identification[playerx][playery]) && (pp_try))
        if (++playerx >= MAXX - 1) {
            playerx = 1;
            if (++playery >= MAXY - 1) {
                playery = 1;
                --pp_try;
            }
        }
    if (pp_try == 0)
        fl_display_message("Failure in positionplayer()\n");
}

void fl_recalculate_armor_class(void) {
    int i, j, k;
    cdesc[AC] = cdesc[FL_MOREDEFENSES];

    if ( cdesc[FL_WEAR] >= 0 )
        switch ( iven[cdesc[FL_WEAR]] ) {
        case OSHIELD:
            cdesc[AC] += 2 + ivenarg[cdesc[FL_WEAR]];
            break;

        case OLEATHER:
            cdesc[AC] += 2 + ivenarg[cdesc[FL_WEAR]];
            break;

        case OSTUDLEATHER:
            cdesc[AC] += 3 + ivenarg[cdesc[FL_WEAR]];
            break;

        case ORING:
            cdesc[AC] += 5 + ivenarg[cdesc[FL_WEAR]];
            break;

        case OCHAIN:
            cdesc[AC] += 6 + ivenarg[cdesc[FL_WEAR]];
            break;

        case OSPLINT:
            cdesc[AC] += 7 + ivenarg[cdesc[FL_WEAR]];
            break;

        case OPLATE:
            cdesc[AC] += 9 + ivenarg[cdesc[FL_WEAR]];
            break;

        case OPLATEARMOR:
            cdesc[AC] += 10 + ivenarg[cdesc[FL_WEAR]];
            break;

        case OSSPLATE:
            cdesc[AC] += 12 + ivenarg[cdesc[FL_WEAR]];
            break;
        }

    if ( cdesc[FL_SHIELD] >= 0 )
        if ( iven[cdesc[FL_SHIELD]] == OSHIELD ) {
            cdesc[AC] += 2 + ivenarg[cdesc[FL_SHIELD]];
        }

    if ( cdesc[FL_WIELD] < 0 ) {
        cdesc[WCLASS] = 0;
    }

    else {
        i = ivenarg[cdesc[FL_WIELD]];

        switch ( iven[cdesc[FL_WIELD]] ) {
        case ODAGGER:
            cdesc[WCLASS] = 3 + i;
            break;

        case OBELT:
            cdesc[WCLASS] = 7 + i;
            break;

        case OSHIELD:
            cdesc[WCLASS] = 8 + i;
            break;

        case OSPEAR:
            cdesc[WCLASS] = 10 + i;
            break;

        case OBATTLEAXE:
            cdesc[WCLASS] = 17 + i;
            break;

        case OGREATSWORD:
            cdesc[WCLASS] = 19 + i;
            break;

        case OLONGSWORD:
            cdesc[WCLASS] = 22 + i;
            break;

        case O2SWORD:
            cdesc[WCLASS] = 26 + i;
            break;

        case OHSWORD:
            cdesc[WCLASS] = 25 + i;
            cdesc[FL_AWARENESS] += 160;
            break;

        case OSWORD:
            cdesc[WCLASS] = 32 + i;
            break;

        case OSWORDofSLASHING:
            cdesc[WCLASS] = 30 + i;
            break;

        case OHAMMER:
            cdesc[WCLASS] = 35 + i;
            break;

        default:
            cdesc[WCLASS] = 0;
        }
    }

    cdesc[WCLASS] += cdesc[MOREDAM];
    /*  now for regeneration abilities based on rings   */
    cdesc[FL_REGEN] = 1;
    cdesc[FL_ENERGY] = 0;
    j = 0;

    for ( k = 25; k > 0; k-- )
        if ( iven[k] ) {
            j = k;
            k = 0;
        }

    for ( i = 0; i <= j; i++ ) {
        switch ( iven[i] ) {
        case OPROTRING:
            cdesc[AC] += ivenarg[i] + 1;
            break;

        case ODAMRING:
            cdesc[WCLASS] += ivenarg[i] + 1;
            break;

        case OBELT:
            cdesc[WCLASS] += ( ( ivenarg[i] << 1 ) ) + 2;
            break;

        case OREGENRING:
            cdesc[FL_REGEN] += ivenarg[i] + 1;
            break;

        case ORINGOFEXTRA:
            cdesc[FL_REGEN] += 5 * ( ivenarg[i] + 1 );
            break;

        case OENERGYRING:
            cdesc[FL_ENERGY] += ivenarg[i] + 1;
            break;
        }
    }
}


/*
* quit()
*
* subroutine to ask if the player really wants to quit
*/
void
quit ( void )
{
    int i;
    fl_termcap_cursor_position(1,24);
    strcpy ( lastmonst, "" );
    fl_display_message ( "\nDo you really want to quit (all progress will be lost)?" );

    for ( ;; ) {
        i = ttgetch ();

        if ( ( i == 'y' ) || ( i == 'Y' ) ) {
            exit(0);
            return;
        }

        if ( ( i == 'n' ) || ( i == 'N' ) || ( i == '\33' ) ) {
            fl_display_message ( " no" );
            fl_output_buffer_flush();
            return;
        }
    }
}

/*
* function to ask --more--. If the user enters a space, returns 0.  If user
* enters Escape, returns 1.  If user enters alphabetic, then returns that
*  value.
*/
int
more ( char select_allowed )
{
    int i;
    fl_display_message ( "\n  --- press " );
    lstandout ( "space" );
    fl_display_message ( " to continue --- " );

    if ( select_allowed ) {
        fl_display_message ( "letter to select --- " );
    }

    for ( ;; ) {
        if ( ( i = ttgetch () ) == ' ' || i == '\n' ) {
            return 0;
        }

        if ( i == '\x1B' ) {
            return 1;
        }

        if ( select_allowed ) {
            if ( isupper ( i ) ) {
                i = tolower ( i );
            }

            if ( ( i >= 'a' && i <= 'z' ) || i == '.' ) {
                return i;
            }
        }
    }
}



/*
* function to enchant armor player is currently wearing
*/
void
enchantarmor ( void )
{
    int tmp;

    if ( cdesc[FL_WEAR] < 0 ) {
        if ( cdesc[FL_SHIELD] < 0 ) {
            fl_termcap_cursor_position(1,24);
            fl_display_message ( "\nYou feel a sense of loss" );
            return;
        }

        else {
            tmp = iven[cdesc[FL_SHIELD]];

            if ( tmp != OSCROLL )
                if ( tmp != OPOTION ) {
                    ivenarg[cdesc[FL_SHIELD]]++;
                    bottomline ();
                }
        }
    }

    tmp = iven[cdesc[FL_WEAR]];

    if ( tmp != OSCROLL )
        if ( tmp != OPOTION ) {
            ivenarg[cdesc[FL_WEAR]]++;
            bottomline ();
        }
}



/*
* function to enchant a weapon presently being wielded
*/
void
enchweapon ( void )
{
    int tmp;

    if ( cdesc[FL_WIELD] < 0 ) {
        fl_termcap_cursor_position(1,24);
        fl_display_message ( "\nYou feel a sense of loss" );
        return;
    }

    tmp = iven[cdesc[FL_WIELD]];

    if ( tmp != OSCROLL )
        if ( tmp != OPOTION ) {
            ivenarg[cdesc[FL_WIELD]]++;

            if ( tmp == OCLEVERRING ) {
                cdesc[INTELLIGENCE]++;
            }

            else if ( tmp == OSTRRING ) {
                cdesc[FL_STREXTRA]++;
            }

            else if ( tmp == ODEXRING ) {
                cdesc[FL_DEXTERITY]++;
            }

            bottomline ();
        }
}



/*
* function to return 1 if a monster is next to the player else returns 0
*/
int
nearbymonst ( void )
{
    int tmp, tmp2;

    for ( tmp = playerx - 1; tmp < playerx + 2; tmp++ )
        for ( tmp2 = playery - 1; tmp2 < playery + 2; tmp2++ )
            if ( monster_identification[tmp][tmp2] ) {
                return ( 1 );  /* if monster nearby */
            }

    return 0;
}



/*
* function to steal an object_identification from the players pockets
* returns 1 if steals something else returns 0
*/
int
stealsomething ( void )
{
    int i, j;
    j = 100;

    for ( ;; ) {
        i = TRund (MAXINVEN);

        if ( iven[i] && cdesc[FL_WEAR] != i &&
                cdesc[FL_WIELD] != i && cdesc[FL_SHIELD] != i ) {
            fl_display_inventory(i,0);
            adjustcvalues ( iven[i], ivenarg[i] );
            iven[i] = 0;
            return 1;
        }

        if ( --j <= 0 ) {
            return 0;
        }
    }
}



/*
* function to return 1 is player carrys nothing else return 0
*/
int
emptyhanded ( void )
{
    int i;

    for ( i = 0; i < 26; i++ ) {
        if ( iven[i] && i != cdesc[FL_WIELD] &&
                i != cdesc[FL_WEAR] && i != cdesc[FL_SHIELD] ) {
            return 0;
        }
    }

    return 1;
}



/*
* function to create a gem on a square near the player
*/
void
creategem ( void )
{
    int i, j;

    switch ( TRnd ( 4 ) ) {
    case 1:
        i = ODIAMOND;
        j = 50;
        break;

    case 2:
        i = ORUBY;
        j = 40;
        break;

    case 3:
        i = OEMERALD;
        j = 30;
        break;

    default:
        i = OSAPPHIRE;
        j = 20;
        break;
    };

    createitem ( i, TRnd ( j ) + j / 10 );
}



/*
* function to change character levels as needed when dropping an object
* that affects these characteristics
*/
void
adjustcvalues ( int itm, int arg )
{
    int flag;
    flag = 0;

    switch ( itm ) {
    case ODEXRING:
        cdesc[FL_DEXTERITY] -= arg + 1;
        flag = 1;
        break;

    case OSTRRING:
        cdesc[FL_STREXTRA] -= arg + 1;
        flag = 1;
        break;

    case OCLEVERRING:
        cdesc[INTELLIGENCE] -= arg + 1;
        flag = 1;
        break;

    case OHAMMER:
        cdesc[FL_DEXTERITY] -= 10;
        cdesc[FL_STREXTRA] -= 10;
        cdesc[INTELLIGENCE] += 10;
        flag = 1;
        break;

    case OSWORDofSLASHING:
        cdesc[FL_DEXTERITY] -= 5;
        flag = 1;
        break;

    case OORBOFDRAGON:
        --cdesc[SLAYING];
        return;

    case OSPIRITSCARAB:
        --cdesc[NEGATESPIRIT];
        return;

    case OCUBEofUNDEAD:
        --cdesc[CUBEofUNDEAD];
        return;

    case ONOTHEFT:
        --cdesc[NOTHEFT];
        return;

    case OGREATSWORD:
        cdesc[GREATSWORDDEATH] = 0;
        return;

    case OPOTION:
    case OSCROLL:
        return;

    default:
        flag = 1;
    };

    if ( flag ) {
        bottomline ();
    }
}

/*
* subroutine to get a yes or no response from the user
* returns y or n
*/
char
getyn ( void )
{
    int i = 0;

    while ( i != 'y' && i != 'n' && i != '\33' ) {
        i = ttgetch ();
    }

    return ( char ) i;
}



/*
* function to calculate the pack weight of the player
* returns the number of pounds the player is carrying
*/
int
packweight ( void )
{
    int i, j = 25, k;
    k = cdesc[GOLD] / 1000;

    while ( ( iven[j] == 0 ) && ( j > 0 ) ) {
        --j;
    }

    for ( i = 0; i <= j; i++ )
        switch ( iven[i] ) {
        case 0:
            break;

        case OSSPLATE:
        case OPLATEARMOR:
            k += 40;
            break;

        case OPLATE:
            k += 35;
            break;

        case OHAMMER:
            k += 30;
            break;

        case OSPLINT:
            k += 26;
            break;

        case OSWORDofSLASHING:
        case OCHAIN:
        case OBATTLEAXE:
        case O2SWORD:
        case OHSWORD:
            k += 23;
            break;

        case OLONGSWORD:
        case OSWORD:
        case ORING:
            k += 20;
            break;

        case OGREATSWORD:
        case OSTUDLEATHER:
            k += 15;
            break;

        case OLEATHER:
        case OSPEAR:
            k += 8;
            break;

        case OORBOFDRAGON:
        case OBELT:
            k += 4;
            break;

        case OSHIELD:
            k += 7;
            break;

        case OCHEST:
            k += 30 + ivenarg[i];
            break;

        default:
            k++;
            break;
        };

    return ( k );
}
