/* diag.c */
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <cstdio>
#include <time.h>

#include "../includes/larncons.h"
#include "../includes/larndata.h"
#include "../includes/larnfunc.h"
#include "../includes/global.h"
#include "../includes/io.h"
#include "../includes/scores.h"
#include "../includes/store.h"
#include "../includes/sysdep.h"
#include "../includes/diag.h"
#include "../includes/create.h"

/*
***************************
DIAG -- dungeon diagnostics
***************************

subroutine to print out data for debugging
*/
static int rndcount[16];
int
diag ( void )
{
	int i, j;
	int hit, dam;
	cursors ();
	lwclose ();

	if ( lcreat ( diagfile ) <
	     0 )  	/*  open the diagnostic file    */
	{
		lcreat ( ( char * ) 0 );
		lprcat ( "\ndiagnostic failure\n" );
		return ( -1 );
	}

	write ( 1, "\nDiagnosing . . .\n", 18 );
	lprcat ( "\n\nBeginning of DIAG diagnostics ----------\n" );
	/*  for the character attributes    */
	lprintf ( "\n\nPlayer attributes:\n\nHit points: %2d(%2d)",
	          ( int ) cdesc[HP],
	          ( int ) cdesc[HPMAX] );
	lprintf ( "\ngold: %d  Experience: %d  Character level: %d  Level in caverns: %d",
	          ( int ) cdesc[GOLD], ( int ) cdesc[EXPERIENCE],
	          ( int ) cdesc[LEVEL],
	          ( int ) level );
	lprintf ( "\nTotal types of monsters: %d",
	          ( int ) MAXMONST + 8 );
	lprcat ( "\f\nHere's the dungeon:\n\n" );
	i = level;

	for ( j = 0; j < MAXLEVEL + MAXVLEVEL; j++ )
	{
		newcavelevel ( j );
		lprintf ( "\nMaze for level %s:\n", levelname[level] );
		diagdrawscreen();
	}

	newcavelevel ( i );
	lprcat ( "\f\nNow for the monster data:\n\n" );
	lprcat
	( "   Monster Name      LEV  AC   DAM  ATT  DEF    GOLD   HP     EXP   \n" );
	lprcat
	( "--------------------------------------------------------------------------\n" );

	for ( i = 0; i <= MAXMONST + 8; i++ )
	{
		lprintf ( "%19s  %2d  %3d ", monster[i].name,
		          ( int ) monster[i].level,
		          ( int ) monster[i].armorclass );
		lprintf ( " %3d  %3d  %3d  ", ( int ) monster[i].damage,
		          ( int ) monster[i].attack, ( int ) monster[i].defense );
		lprintf ( "%6d  %3d   %6d\n", ( int ) monster[i].gold,
		          ( int ) monster[i].hitpoints, ( int ) monster[i].experience );
	}

	lprcat ( "\n\nHere's a Table for the to hit percentages\n" );
	lprcat
	( "\n     We will be assuming that players level = 2 * monster level" );
	lprcat ( "\n     and that the players dexterity and strength are 16." );
	lprcat
	( "\n    to hit: if (rnd(22) < (2[monst AC] + your level + dex + WC/8 -1)/2) then hit" );
	lprcat ( "\n    damage = rund(8) + WC/2 + STR - cdesc[HARDGAME] - 4" );
	lprcat ( "\n    to hit:  if rnd(22) < to hit  then player hits\n" );
	lprcat
	( "\n    Each entry is as follows:  to hit / damage / number hits to kill\n" );
	lprcat ( "\n          monster     WC = 4         WC = 20        WC = 40" );
	lprcat
	( "\n---------------------------------------------------------------" );

	for ( i = 0; i <= MAXMONST + 8; i++ )
	{
		hit = 2 * monster[i].armorclass + 2 * monster[i].level + 16;
		dam = 16 - cdesc[HARDGAME];
		lprintf ( "\n%20s   %2d/%2d/%2d       %2d/%2d/%2d       %2d/%2d/%2d",
		          monster[i].name,
		          ( int ) ( hit / 2 ), ( int ) max ( 0, dam + 2 ),
		          ( int ) ( monster[i].hitpoints / ( dam + 2 ) + 1 ),
		          ( int ) ( ( hit + 2 ) / 2 ), ( int ) max ( 0, dam + 10 ),
		          ( int ) ( monster[i].hitpoints / ( dam + 10 ) + 1 ),
		          ( int ) ( ( hit + 5 ) / 2 ), ( int ) max ( 0, dam + 20 ),
		          ( int ) ( monster[i].hitpoints / ( dam + 20 ) + 1 ) );
	}

	lprcat ( "\n\nHere's the list of available potions:\n\n" );

	for ( i = 0; i < MAXPOTION; i++ )
	{
		lprintf ( "%20s\n", &potionname[i][1] );
	}

	lprcat ( "\n\nHere's the list of available scrolls:\n\n" );

	for ( i = 0; i < MAXSCROLL; i++ )
	{
		lprintf ( "%20s\n", &scrollname[i][1] );
	}

	lprcat ( "\n\nHere's the spell list:\n\n" );
	lprcat ( "spell          name           description\n" );
	lprcat
	( "-------------------------------------------------------------------------------------------\n\n" );

	for ( j = 0; j < SPNUM; j++ )
	{
		lprc ( ' ' );
		lprcat ( spelcode[j] );
		lprintf ( " %21s  %s\n", spelname[j], speldescript[j] );
	}

	lprcat ( "\n\nFor the cdesc[] array:\n" );

	for ( j = 0; j < 100; j += 10 )
	{
		lprintf ( "\nc[%2d] = ", ( int ) j );

		for ( i = 0; i < 9; i++ )
		{
			lprintf ( "%5d ", ( int ) cdesc[i + j] );
		}
	}

	lprcat ( "\n\nTest of random number generator ----------------" );
	lprcat ( "\n    for 25,000 calls divided into 16 slots\n\n" );

	for ( i = 0; i < 16; i++ )
	{
		rndcount[i] = 0;
	}

	for ( i = 0; i < 25000; i++ )
	{
		rndcount[rund ( 16 )]++;
	}

	for ( i = 0; i < 16; i++ )
	{
		lprintf ( "  %5d", ( int ) rndcount[i] );

		if ( i == 7 )
		{
			lprc ( '\n' );
		}
	}

	lprcat ( "\n\n" );
	lwclose ();
	lcreat ( ( char * ) 0 );
	lprcat ( "Done Diagnosing . . ." );
	return ( 0 );
}

/*
subroutine to draw the whole screen as the player knows it
*/
void
diagdrawscreen ( void )
{
	int i, j, k;

	for ( i = 0; i < MAXY; i++ )
		/*  for the east west walls of this line    */
	{
		for ( j = 0; j < MAXX; j++ )
			if ( ( k = mitem[j][i] ) )
			{
				lprc ( monstnamelist[k] );
			}

			else
			{
				lprc ( objnamelist[item[j][i]] );
			}

		lprc ( '\n' );
	}
}
