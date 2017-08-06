/*
Larn was distributed to the net for the enjoyment of all.  One of my goals
is to have larn available to whom ever wants to play it.  I therefore give
permission to use the sources, to modify the sources, or to port the sources to
another machine, provided that a profit is not made from larn or its sources,
or the aforementioned activities.  Should a profit be made without permissions,
I will exercise my copyright.

Other than this case, enjoy it!

For more information, see LICENSE.txt in the 'doc' folder.
*/

/*
action.c

Routines to perform the actual actions associated with various
player entered commands.

act_remove_gems         remove gems from a throne
act_sit_throne          sit on a throne
act_up_stairs           go up stairs
act_down_stairs         go down stairs
act_drink_fountain      drink from a fountain
act_wash_fountain       wash at a fountain
fl_act_exit_temple
fl_act_enter_temple
fl_temple_actions
act_desecrate_altar     desecrate an altar
act_donation_pray       pray, donating money
act_just_pray           pray, not donating money
act_prayer_heard        prayer was heard
act_ignore_altar        ignore an altar
act_open_chest          open a chest
act_open_door           open a door
*/

#include <iostream>
#include <cstdlib>
#include <curses.h>
#include "../includes/action.h"
#include "config/data.h"
#include "templates/math.t.hpp"
#include "dungeon/dungeon.hpp"
#include "../includes/display.h"
#include "../includes/global.h"
#include "../includes/io.h"
#include "../includes/main.h"
#include "../includes/monster.h"
#include "../includes/moreobj.h"
#include "../includes/object.h"
#include "core/scores.hpp"
#include "core/sysdep.hpp"
#include "core/funcs.hpp"

using std::cout;

static void fl_temple_actions ( int );

static void act_prayer_heard ( void );




/*
act_remove_gems

Remove gems from a throne.

arg is zero if there is a gnome king associated with the throne

Assumes that cursors() has been called previously, and that a check
has been made that the throne actually has gems.
*/
void
act_remove_gems ( int arg )
{
    int i, k;
    k = TRnd ( 101 );

    if ( k < 25 ) {
        for ( i = 0; i < TRnd ( 4 ); i++ ) {
            /* gems pop ohf the throne */
            creategem ();
        }

        object_identification[playerx][playery] = ODEADTHRONE;
        been_here_before[playerx][playery] = 0;
    }

    else if ( k < 40 && arg == 0 ) {
        createmonster ( GNOMEKING );
        object_identification[playerx][playery] = OTHRONE2;
        been_here_before[playerx][playery] = 0;
    }

    else {
        fl_display_message ( "\nNothing happens" );
    }
}



/*
act_sit_throne

Sit on a throne.

arg is zero if there is a gnome king associated with the throne

Assumes that cursors() has been called previously.
*/
void
act_sit_throne ( int arg )
{
    int k;
    k = TRnd ( 101 );

    if ( k < 30 && arg == 0 ) {
        createmonster ( GNOMEKING );
        object_identification[playerx][playery] = OTHRONE2;
        been_here_before[playerx][playery] = 0;
    }

    else if ( k < 35 ) {
        fl_display_message ( "\nZaaaappp!  You've been teleported!\n" );
        fl_teleport ( 0 );
    }

    else {
        fl_display_message ( "\nNothing happens" );
    }
}



/*
assumes that cursors() has been called and that a check has been made that
the user is actually standing at a set of up stairs.
*/
void
act_up_stairs ( void )
{
    if ( level >= 2 && level != 11 ) {
        newcavelevel ( level - 1 );
        draws ( 0, MAXX, 0, MAXY );
        bot_linex ();
        refresh();
    }

    else {
        fl_display_message ( "\nThe stairs lead to a dead end!" );
    }
}



/*
assumes that cursors() has been called and that a check has been made that
the user is actually standing at a set of down stairs.
*/
void
act_down_stairs ( void )
{
    if ( level != 0 && level != 10 && level != 13 ) {
        newcavelevel ( level + 1 );
        draws ( 0, MAXX, 0, MAXY );
        bot_linex ();
        refresh();
    }

    else {
        fl_display_message ( "\nThe stairs lead to a dead end!" );
    }
}



/*
Code to perform the action of drinking at a fountian.  Assumes that
cursors() has already been called, and that a check has been made that
the player is actually standing at a live fountain.
*/
void
act_drink_fountain ( void )
{
    int x;

    if ( TRnd ( 1501 ) < 2 ) {
        fl_display_message ( "\nOops!  You seem to have caught the dreadful sleep!" );
        fl_output_buffer_flush ();
        fl_wait ( FL_WAIT_DURATION );
        fl_player_death ( 280 );
        return;
    }

    x = TRnd ( 100 );

    if ( x < 7 ) {
        cdesc[FL_HALFDAM] += 200 + TRnd ( 200 );
        fl_display_message ( "\nYou feel a sickness coming on" );
    }

    else if ( x < 13 )
        fl_drink_potion ( 23,
                      0 );	/* see invisible,but don't been_here_before the potion */
    else if ( x < 45 ) {
        fl_display_message ( "\nnothing seems to have happened" );
    }

    else if ( TRnd ( 3 ) != 2 ) {
        fntchange ( 1 );  /*  change char levels upward   */
    }

    else {
        fntchange ( -1 );  /*  change char levels downward */
    }

    if ( TRnd ( 12 ) < 3 ) {
        fl_display_message ( "\nThe fountains bubbling slowly quiets" );
        object_identification[playerx][playery] = ODEADFOUNTAIN;	/* dead fountain */
        been_here_before[playerx][playery] = 0;
    }
	
	if (TRnd(10) < (6)) {
		if (cdesc[HUNGER] < 100) {
			fl_display_message("\nThe refreshing water makes you a little less hungry");
			cdesc[HUNGER] += 5;
		} else {
			fl_display_message("\nThe refreshing water gives you strength!");
			cdesc[FL_STRENGTH] += 1;
		}
	}
    return;
}

/*
Code to perform the action of washing at a fountain.  Assumes that
cursors() has already been called and that a check has been made that
the player is actually standing at a live fountain.
*/
void
act_wash_fountain ( void )
{
	FLCoreFuncs CoreFuncs;
    int x;

    if ( TRnd ( 100 ) < 11 ) {
        x = TRnd ( ( level << 2 ) + 2 );
        lprintf ( "\nOh no!  The water was foul!  You suffer %d hit points!",
                  ( int ) x );
        lastnum = 273;
        CoreFuncs.DecreasePHealth ( x );
        bottomline ();
        fl_termcap_cursor_position(1,24);
    }

    else if ( TRnd ( 100 ) < 29 ) {
        fl_display_message ( "\nYou got the dirt off!" );
    }

    else if ( TRnd ( 100 ) < 31 )
        fl_display_message
        ( "\nThis water seems to be hard water!  The dirt didn't come off!" );

    else if ( TRnd ( 100 ) < 34 ) {
        createmonster ( WATERLORD );  /*    make water lord     */
    }

    else {
        fl_display_message ( "\nnothing seems to have happened" );
    }
    return;
}

void fl_act_enter_temple(void)
{
	FLCoreFuncs CoreFuncs;
	
    if (cdesc[FL_STRENGTH] < 20 && TRnd(42)) {
        fl_display_message("\nThe temple doors begin to burn you");
        lastnum = 275;
        CoreFuncs.DecreasePHealth(50 + TRnd(26));
        bottomhp();
    }
    newcavelevel(MAXLEVEL);
    draws(0,MAXX,0,MAXY);
    bot_linex();
    return;
}

void
fl_act_exit_temple ( void )
{
	FLCoreFuncs CoreFuncs;
    if ( level != 11 ) {
        fl_display_message
        ( "\nThese doors cannot be opened." );
        return;
    }
    if (cdesc[FL_STRENGTH] < 20 + TRnd(2)) {
        fl_display_message ( "\nYou suddenly feel weaker." );
        lastnum = 275;
        CoreFuncs.DecreasePHealth ( 15 + TRnd ( 20 ) );
        bottomhp ();
        return;
    }

    fl_output_buffer_flush ();
    newcavelevel ( 0 );
    fl_temple_actions ( FL_OBJECT_TEMPLE_IN );
    return;
}

static void
fl_temple_actions ( int object )
{
    int i, j;

    /* place player near the volcanic shaft */
    for ( i = 0; i < MAXY; i++ )
        for ( j = 0; j < MAXX; j++ )
            if ( object_identification[j][i] == object ) {
                playerx = j;
                playery = i;
                positionplayer ();
                i = MAXY;
                break;
            }

    draws ( 0, MAXX, 0, MAXY );
    bot_linex ();
    return;
}



/*
Perform the actions associated with Altar desecration.
*/
void
act_desecrate_altar ( void )
{
	FLCoreFuncs CoreFuncs;
    if ( TRnd ( 100 ) < 60 ) {
        createmonster ( makemonst ( level + 2 ) + 8 );
        fl_display_message ( "\nThe altar explodes violently and spawns a monster!" );
        CoreFuncs.DecreasePHealth (5);
        cdesc[FL_AGGRAVATE] += 2500;
        forget ();
    }

    else if ( TRnd ( 101 ) < 30 ) {
        fl_display_message ( "\nThe altar crumbles into a pile of dust before your eyes" );
        forget ();		/*  remember to destroy the altar   */
    }

    else {
        fl_display_message ( "\nThe altar was desecrated and ruined!" );
        forget();
    }

    return;
}



/*
Perform the actions associated with praying at an altar and giving a
donation.
*/
void
act_donation_pray ( void )
{
    int k, temp;

    for ( ;; ) {
        fl_display_message ( "\n\n" );
        fl_termcap_cursor_position( 1, 24 );
        CLEAR_EOL ();
        fl_termcap_cursor_position( 1, 23 );
        CLEAR_EOL ();
        fl_display_message ( "how much do you donate? " );
        k = readnum ( ( int ) cdesc[GOLD] );
        fl_display_message ( "\n" );

        /* make giving zero gold equivalent to 'just pray'ing.  Allows player to
           'just pray' in command mode, without having to add yet another command.
         */
        if ( k == 0 ) {
            act_just_pray ();
            return;
        }

        if ( cdesc[GOLD] >= k ) {
            temp = cdesc[GOLD] / 10;
            cdesc[GOLD] -= k;
            bottomline ();

            /* if player gave less than 10% of _original_ gold, make a monster
             */
            if ( k < temp || k < TRnd ( 50 ) ) {
                /* added by ~Gibbon */
                fl_display_message("You have offended the Gods.");
                createmonster ( makemonst ( level + 1 ) );
                cdesc[FL_AGGRAVATE] += 200;
                return;
            }

            if ( TRnd ( 101 ) > 50 ) {
                act_prayer_heard ();
                return;
            }

            if ( TRnd ( 43 ) == 5 ) {
                if ( cdesc[FL_WEAR] ) {
                    fl_display_message ( "You feel your armor vibrate for a moment" );
                }

                enchantarmor ();
                return;
            }

            if ( TRnd ( 43 ) == 8 ) {
                if ( cdesc[FL_WIELD] ) {
                    fl_display_message ( "You feel your weapon vibrate for a moment" );
                }

                enchweapon ();
                return;
            }

            fl_display_message("The Gods thank you for your prayers.");
            return;
        }

        /* Player donates more gold than they have.  Loop back around so
           player can't escape the altar for free.
         */
        fl_display_message ( "You don't have that much!" );
    }
    forget();
}



/*
Performs the actions associated with 'just praying' at the altar.  Called
when the user responds 'just pray' when in prompt mode, or enters 0 to
the money prompt when praying.

Assumes cursors(), and that any leading \n have been printed
*/
void
act_just_pray ( void )
{
    if ( TRnd ( 100 ) < 75 ) {
        fl_display_message ( "The altar ascends!" );
    }

    else if ( TRnd ( 43 ) == 10 ) {
        if ( cdesc[FL_WEAR] ) {
            fl_display_message ( "You feel your armor vibrate for a moment" );
        }

        enchantarmor ();
        return;
    }

    else if ( TRnd ( 43 ) == 10 ) {
        if ( cdesc[FL_WIELD] ) {
            fl_display_message ( "You feel your weapon vibrate for a moment" );
        }

        enchweapon ();
        return;
    }

    else {
        createmonster ( makemonst ( level + 1 ) );
    }

    return;
    forget();
}

/*
Performs the actions associated with 'give thanks' at the altar.  Called
when the user responds 'give thanks (t)' when in prompt mode.

Assumes cursors(), and that any leading \n have been printed

~Gibbon
*/
void
act_give_thanks(void)
{
    FLCoreFuncs CoreFuncs;
    int i;

    //The below math is to identify if the player has the ghani statue in their inventory or not. ~Gibbon
    for ( i = 0; i < MAXOBJECT; i++ )

        /*
         * If the player is holding the Ghani statue, when praying at an altar,
         * they will get some goodies.
         *
         * ~Gibbon
        */

        if (iven[i] == OGHANISTATUE) {
            fl_display_message("You place the statues on the altar and close your eyes reverently..");
            fl_display_message("\nWe hear your prayers.\nBehold your increased strength and experience!");
            cdesc[FL_STRENGTH] += 4;
            CoreFuncs.IncreaseExperience(600);
            //Then we remove the statue from the inventory.  If the user has more than 1 all will be removed. ~Gibbon
            iven[i] = 0;
            forget();
        }
    return;
}

/*
* function to cast a +3 protection on the player
*/
static void
act_prayer_heard ( void )
{
    fl_display_message ( "You have been heard!" );

    if ( cdesc[FL_ALTPRO] == 0 ) {
        cdesc[FL_MOREDEFENSES] += 3;
    }

    /* protection field */
    cdesc[FL_ALTPRO] += 500;
    bottomline ();
    forget();
}

/*
Performs the act of ignoring an altar.

Assumptions:  cursors() has been called.
*/
void
act_ignore_altar ( void )
{
    if ( TRnd ( 100 ) < 30 ) {
        createmonster ( makemonst ( level + 1 ) );
        fl_display_message("The altar turns into a monster!");
        cdesc[FL_AGGRAVATE] += TRnd ( 450 );
        forget();
    }

    else {
        fl_display_message ( "\nThe altar crumbles into stone!" );
        forget();
    }

    return;
}



/*
Performs the act of opening a chest.

Parameters:   x,y location of the chest to open.
Assumptions:  cursors() has been called previously
*/
void
act_open_chest ( int x, int y )
{
    int i, k;
    k = TRnd ( 101 );

    if ( k < 40 ) {
        fl_display_message ( "\nThe chest explodes as you open it" );
        i = TRnd ( 10 );
        lastnum = 281;		/* in case he dies */
        lprintf ( "\nYou suffer %d hit points damage!", ( int ) i );
        checkloss ( i );

        switch ( TRnd ( 10 ) ) {	/* see if he gets a curse */
        case 1:
            cdesc[FL_ITCHING] += TRnd ( 1000 ) + 100;
            fl_display_message ( "\nYou feel an irritation spread over your skin!" );
            break;

        case 2:
            cdesc[FL_CLUMSINESS] += TRnd ( 1600 ) + 200;
            fl_display_message ( "\nYou begin to lose hand to eye coordination!" );
            break;

        case 3:
            cdesc[FL_HALFDAM] += TRnd ( 1600 ) + 200;
            fl_display_message ( "\nA sickness engulfs you!" );
            break;
        };

        object_identification[x][y] = been_here_before[x][y] = 0;	/* destroy the chest */

        if ( TRnd ( 100 ) < 69 ) {
            creategem ();  /* gems from the chest */
        }

        dropgold ( TRnd ( 110 * object_argument[playerx][playery] + 200 ) );

        for ( i = 0; i < TRnd ( 4 ); i++ ) {
            something ( object_argument[playerx][playery] + 2 );
        }
    }

    else {
        fl_display_message ( "\nNothing happens" );
    }

    return;
}



/*
Perform the actions common to command and prompt mode when opening a
door.  Assumes cursors().

Parameters:     the X,Y location of the door to open.
Return value:   TRUE if successful in opening the door, false if not.
*/
int
act_open_door ( int x, int y )
{
    FLCoreFuncs CoreFuncs;
    if ( TRnd ( 11 ) < 7 ) {
        switch ( object_argument[x][y] ) {
        case 6:
            fl_display_message ( "\nThe door makes an awful groan, but remains stuck" );
            cdesc[FL_AGGRAVATE] += TRnd ( 400 );
            break;

        case 7:
            fl_display_message ( "\nYou are jolted by an electric shock" );
            lastnum = 274;
            CoreFuncs.DecreasePHealth ( TRnd ( 20 ) );
            bottomline ();
            break;

        case 8:
            fl_display_message ( "\nYou feel drained" );
            CoreFuncs.DecreasePlayerLevel();
            break;

        case 9:
            fl_display_message ( "\nYou suddenly feel weaker" );

            if ( cdesc[FL_STRENGTH] > 3 ) {
                cdesc[FL_STRENGTH]--;
            }

            bottomline ();
            break;

        default:
            fl_display_message ( "\nThe door makes an awful groan, but remains stuck" );
            break;
        }

        return ( 0 );
    }

    else {
        fl_display_message ( "\nThe door opens" );
        been_here_before[x][y] = 0;
        object_identification[x][y] = OOPENDOOR;
        return ( 1 );
    }
}
