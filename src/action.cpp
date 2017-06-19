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
act_up_shaft            up volcanic shaft
act_down_shaft          down volcanic shaft
volshaft_climbed        place player near volcanic shaft
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
#include "config/larncons.h"
#include "config/data.h"
#include "config/larnfunc.h"
#include "../includes/create.h"
#include "../includes/display.h"
#include "../includes/global.h"
#include "../includes/io.h"
#include "../includes/main.h"
#include "../includes/monster.h"
#include "../includes/moreobj.h"
#include "../includes/object.h"
#include "../includes/scores.h"
#include "../includes/sysdep.h"

using std::cout;

static void volshaft_climbed ( int );

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

	if ( k < 25 )
	{
		for ( i = 0; i < TRnd ( 4 ); i++ )
		{
			/* gems pop ohf the throne */
			creategem ();
		}

		item[playerx][playery] = ODEADTHRONE;
		know[playerx][playery] = 0;
	}

	else
		if ( k < 40 && arg == 0 )
		{
			createmonster ( GNOMEKING );
			item[playerx][playery] = OTHRONE2;
			know[playerx][playery] = 0;
		}

		else
		{
			lprcat ( "\nNothing happens" );
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

	if ( k < 30 && arg == 0 )
	{
		createmonster ( GNOMEKING );
		item[playerx][playery] = OTHRONE2;
		know[playerx][playery] = 0;
	}

	else
		if ( k < 35 )
		{
			lprcat ( "\nZaaaappp!  You've been teleported!\n" );
			oteleport ( 0 );
		}

		else
		{
			lprcat ( "\nNothing happens" );
		}
}



/*
assumes that cursors() has been called and that a check has been made that
the user is actually standing at a set of up stairs.
*/
void
act_up_stairs ( void )
{
	if ( level >= 2 && level != 11 )
	{
		newcavelevel ( level - 1 );
		draws ( 0, MAXX, 0, MAXY );
		bot_linex ();
		refresh();
	}

	else
	{
		lprcat ( "\nThe stairs lead to a dead end!" );
	}
}



/*
assumes that cursors() has been called and that a check has been made that
the user is actually standing at a set of down stairs.
*/
void
act_down_stairs ( void )
{
	if ( level != 0 && level != 10 && level != 13 )
	{
		newcavelevel ( level + 1 );
		draws ( 0, MAXX, 0, MAXY );
		bot_linex ();
		refresh();
	}

	else
	{
		lprcat ( "\nThe stairs lead to a dead end!" );
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

	if ( TRnd ( 1501 ) < 2 )
	{
		lprcat ( "\nOops!  You seem to have caught the dreadful sleep!" );
		lflush ();
		nap ( NAPTIME );
		died ( 280 );
		return;
	}

	x = TRnd ( 100 );

	if ( x < 7 )
	{
		cdesc[HALFDAM] += 200 + TRnd ( 200 );
		lprcat ( "\nYou feel a sickness coming on" );
	}

	else
		if ( x < 13 )
			quaffpotion ( 23,
			              FALSE );	/* see invisible,but don't know the potion */
		else
			if ( x < 45 )
			{
				lprcat ( "\nnothing seems to have happened" );
			}

			else
				if ( TRnd ( 3 ) != 2 )
				{
					fntchange ( 1 );  /*  change char levels upward   */
				}

				else
				{
					fntchange ( -1 );  /*  change char levels downward */
				}

	if ( TRnd ( 12 ) < 3 )
	{
		lprcat ( "\nThe fountains bubbling slowly quiets" );
		item[playerx][playery] = ODEADFOUNTAIN;	/* dead fountain */
		know[playerx][playery] = 0;
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
	int x;

	if ( TRnd ( 100 ) < 11 )
	{
		x = TRnd ( ( level << 2 ) + 2 );
		lprintf ( "\nOh no!  The water was foul!  You suffer %d hit points!",
		          ( int ) x );
		lastnum = 273;
		losehp ( x );
		bottomline ();
		cursors ();
	}

	else
		if ( TRnd ( 100 ) < 29 )
		{
			lprcat ( "\nYou got the dirt off!" );
		}

		else
			if ( TRnd ( 100 ) < 31 )
				lprcat
				( "\nThis water seems to be hard water!  The dirt didn't come off!" );

			else
				if ( TRnd ( 100 ) < 34 )
				{
					createmonster ( WATERLORD );  /*    make water lord     */
				}

				else
				{
					lprcat ( "\nnothing seems to have happened" );
				}

	return;
}



/*
Perform the act of climbing down the volcanic shaft.  Assumes
cursors() has been called and that a check has been made that
are actually at a down shaft.
*/
void
act_down_shaft ( void )
{
	if ( level != 0 )
	{
		lprcat ( "\nThe hobbit hole only extends 5 feet downward!" );
		return;
	}

	if ( packweight () > 45 + 3 * ( cdesc[STRENGTH] +
	                                cdesc[STREXTRA] ) )
	{
		lprcat ( "\nYou slip down the hobbit hole" );
		lastnum = 275;
		losehp ( 30 + TRnd ( 20 ) );
		bottomhp ();
	}

	newcavelevel ( MAXLEVEL );
	draws ( 0, MAXX, 0, MAXY );
	bot_linex ();
	return;
}



/*
Perform the action of climbing up the volcanic shaft. Assumes
cursors() has been called and that a check has been made that
are actually at an up shaft.

*/
void
act_up_shaft ( void )
{
	if ( level != 11 )
	{
		lprcat
		( "\nThe hobbit hole only extends 8 feet upwards before you find a blockage!" );
		return;
	}

	if ( packweight () > 45 + 5 * ( cdesc[STRENGTH] +
	                                cdesc[STREXTRA] ) )
	{
		lprcat ( "\nDown the hobbit hole!" );
		lastnum = 275;
		losehp ( 15 + TRnd ( 20 ) );
		bottomhp ();
		return;
	}

	lflush ();
	newcavelevel ( 0 );
	volshaft_climbed ( OVOLDOWN );
	return;
}



/*
Perform the action of placing the player near the volcanic shaft
after it has been climbed.

Takes one parameter:  the volcanic shaft object to be found.  If have
climbed up, search for OVOLDOWN, otherwise search for OVOLUP.
*/
static void
volshaft_climbed ( int object )
{
	int i, j;

	/* place player near the volcanic shaft */
	for ( i = 0; i < MAXY; i++ )
		for ( j = 0; j < MAXX; j++ )
			if ( item[j][i] == object )
			{
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
	if ( TRnd ( 100 ) < 60 )
	{
		createmonster ( makemonst ( level + 2 ) + 8 );
		lprcat ( "\nThe altar explodes violently and spawns a monster!" );
		losehp (5);
		cdesc[AGGRAVATE] += 2500;
		forget ();
	}

	else
		if ( TRnd ( 101 ) < 30 )
		{
			lprcat ( "\nThe altar crumbles into a pile of dust before your eyes" );
			forget ();		/*  remember to destroy the altar   */
		}

		else
		{
			lprcat ( "\nThe altar was desecrated and ruined!" );
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

	for ( ;; )
	{
		lprcat ( "\n\n" );
		cursor ( 1, 24 );
		CLEAR_EOL ();
		cursor ( 1, 23 );
		CLEAR_EOL ();
		lprcat ( "how much do you donate? " );
		k = readnum ( ( int ) cdesc[GOLD] );
		lprcat ( "\n" );

		/* make giving zero gold equivalent to 'just pray'ing.  Allows player to
		   'just pray' in command mode, without having to add yet another command.
		 */
		if ( k == 0 )
		{
			act_just_pray ();
			return;
		}

		if ( cdesc[GOLD] >= k )
		{
			temp = cdesc[GOLD] / 10;
			cdesc[GOLD] -= k;
			bottomline ();

			/* if player gave less than 10% of _original_ gold, make a monster
			 */
			if ( k < temp || k < TRnd ( 50 ) )
			{
				/* added by ~Gibbon */
				lprcat("You have offended the Gods.");
				createmonster ( makemonst ( level + 1 ) );
				cdesc[AGGRAVATE] += 200;
				return;
			}

			if ( TRnd ( 101 ) > 50 )
			{
				act_prayer_heard ();
				return;
			}

			if ( TRnd ( 43 ) == 5 )
			{
				if ( cdesc[WEAR] )
				{
					lprcat ( "You feel your armor vibrate for a moment" );
				}

				enchantarmor ();
				return;
			}

			if ( TRnd ( 43 ) == 8 )
			{
				if ( cdesc[WIELD] )
				{
					lprcat ( "You feel your weapon vibrate for a moment" );
				}

				enchweapon ();
				return;
			}

			lprcat("The Gods thank you for your prayers.");
			return;
		}

		/* Player donates more gold than they have.  Loop back around so
		   player can't escape the altar for free.
		 */
		lprcat ( "You don't have that much!" );
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
	if ( TRnd ( 100 ) < 75 )
	{
		lprcat ( "The altar ascends!" );
	}

	else
		if ( TRnd ( 43 ) == 10 )
		{
			if ( cdesc[WEAR] )
			{
				lprcat ( "You feel your armor vibrate for a moment" );
			}

			enchantarmor ();
			return;
		}

		else
			if ( TRnd ( 43 ) == 10 )
			{
				if ( cdesc[WIELD] )
				{
					lprcat ( "You feel your weapon vibrate for a moment" );
				}

				enchweapon ();
				return;
			}

			else
			{
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
	int i;
	
	//The below math is to identify if the player has the ghani statue in their inventory or not. ~Gibbon
	for ( i = 0; i < 86; i++ )
	
	/*
	 * If the player is holding the Ghani statue, when praying at an altar,
	 * they will get some goodies.
	 *
	 * ~Gibbon
	*/ 
		
	if (iven[i] == OGHANISTATUE)
		{
				lprcat("You place the statues on the altar and close your eyes reverently..");
				lprcat("\nWe hear your prayers.\nBehold your increased strength and experience!");
				cdesc[STRENGTH] += 4;
				raiseexperience(600);
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
	lprcat ( "You have been heard!" );

	if ( cdesc[ALTPRO] == 0 )
	{
		cdesc[MOREDEFENSES] += 3;
	}

	/* protection field */
	cdesc[ALTPRO] += 500;
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
	if ( TRnd ( 100 ) < 30 )
	{
		createmonster ( makemonst ( level + 1 ) );
		lprcat("The altar turns into a monster!");
		cdesc[AGGRAVATE] += TRnd ( 450 );
		forget();
	}

	else
	{
		lprcat ( "\nThe altar crumbles into stone!" );
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

	if ( k < 40 )
	{
		lprcat ( "\nThe chest explodes as you open it" );
		i = TRnd ( 10 );
		lastnum = 281;		/* in case he dies */
		lprintf ( "\nYou suffer %d hit points damage!", ( int ) i );
		checkloss ( i );

		switch ( TRnd ( 10 ) )  	/* see if he gets a curse */
		{
			case 1:
				cdesc[ITCHING] += TRnd ( 1000 ) + 100;
				lprcat ( "\nYou feel an irritation spread over your skin!" );
				break;

			case 2:
				cdesc[CLUMSINESS] += TRnd ( 1600 ) + 200;
				lprcat ( "\nYou begin to lose hand to eye coordination!" );
				break;

			case 3:
				cdesc[HALFDAM] += TRnd ( 1600 ) + 200;
				lprcat ( "\nA sickness engulfs you!" );
				break;
		};

		item[x][y] = know[x][y] = 0;	/* destroy the chest */

		if ( TRnd ( 100 ) < 69 )
		{
			creategem ();  /* gems from the chest */
		}

		dropgold ( TRnd ( 110 * iarg[playerx][playery] + 200 ) );

		for ( i = 0; i < TRnd ( 4 ); i++ )
		{
			something ( iarg[playerx][playery] + 2 );
		}
	}

	else
	{
		lprcat ( "\nNothing happens" );
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
	if ( TRnd ( 11 ) < 7 )
	{
		switch ( iarg[x][y] )
		{
			case 6:
				lprcat ( "\nThe door makes an awful groan, but remains stuck" );
				cdesc[AGGRAVATE] += TRnd ( 400 );
				break;

			case 7:
				lprcat ( "\nYou are jolted by an electric shock" );
				lastnum = 274;
				losehp ( TRnd ( 20 ) );
				bottomline ();
				break;

			case 8:
				lprcat ( "\nYou feel drained" );
				loselevel ();
				break;

			case 9:
				lprcat ( "\nYou suddenly feel weaker" );

				if ( cdesc[STRENGTH] > 3 )
				{
					cdesc[STRENGTH]--;
				}

				bottomline ();
				break;

			default:
				lprcat ( "\nThe door makes an awful groan, but remains stuck" );
				break;
		}

		return ( 0 );
	}

	else
	{
		lprcat ( "\nThe door opens" );
		know[x][y] = 0;
		item[x][y] = OOPENDOOR;
		return ( 1 );
	}
}
