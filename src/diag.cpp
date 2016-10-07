/* diag.c */
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <cstdio>
#include <time.h>

#include "includes/larncons.h"
#include "includes/larndata.h"
#include "includes/larnfunc.h"
#include "includes/global.h"
#include "includes/io.h"
#include "includes/scores.h"
#include "includes/store.h"
#include "includes/sysdep.h"
#include "includes/diag.h"

#ifdef ANTICHEAT
static void greedy (void);
static void fsorry (void);
static void fcheat (void);
#endif


/*
***************************
DIAG -- dungeon diagnostics
***************************

subroutine to print out data for debugging
*/
#ifdef EXTRA
static int rndcount[16];

int
diag (void)
{
	int i, j;
	int hit, dam;
	cursors ();
	lwclose ();
	if (lcreat (diagfile) <
	    0) {	/*  open the diagnostic file    */
		lcreat ((char *) 0);
		printw ("\ndiagnostic failure\n");
		return (-1);
	}
	write (1, "\nDiagnosing . . .\n", 18);
	printw ("\n\nBeginning of DIAG diagnostics ----------\n");
	/*  for the character attributes    */
	lprintf ("\n\nPlayer attributes:\n\nHit points: %2d(%2d)",
	         (int) cdesc[HP],
	         (int) cdesc[HPMAX]);
	lprintf
	("\ngold: %d  Experience: %d  Character level: %d  Level in caverns: %d",
	 (int) cdesc[GOLD], (int) cdesc[EXPERIENCE],
	 (int) cdesc[LEVEL],
	 (int) level);
	lprintf ("\nTotal types of monsters: %d",
	         (int) MAXMONST + 8);
	printw ("\f\nHere's the dungeon:\n\n");
	i = level;
	for (j = 0; j < MAXLEVEL + MAXVLEVEL; j++) {
		newcavelevel (j);
		lprintf ("\nMaze for level %s:\n", levelname[level]);
		diagdrawscreen ();
	}
	newcavelevel (i);
	printw ("\f\nNow for the monster data:\n\n");
	printw
	("   Monster Name      LEV  AC   DAM  ATT  DEF    GOLD   HP     EXP   \n");
	printw
	("--------------------------------------------------------------------------\n");
	for (i = 0; i <= MAXMONST + 8; i++) {
		lprintf ("%19s  %2d  %3d ", monster[i].name,
		         (int) monster[i].level,
		         (int) monster[i].armorclass);
		lprintf (" %3d  %3d  %3d  ", (int) monster[i].damage,
		         (int) monster[i].attack, (int) monster[i].defense);
		lprintf ("%6d  %3d   %6d\n", (int) monster[i].gold,
		         (int) monster[i].hitpoints, (int) monster[i].experience);
	}
	printw ("\n\nHere's a Table for the to hit percentages\n");
	printw
	("\n     We will be assuming that players level = 2 * monster level");
	printw ("\n     and that the players dexterity and strength are 16.");
	printw
	("\n    to hit: if (rnd(22) < (2[monst AC] + your level + dex + WC/8 -1)/2) then hit");
	printw ("\n    damage = rund(8) + WC/2 + STR - cdesc[HARDGAME] - 4");
	printw ("\n    to hit:  if rnd(22) < to hit  then player hits\n");
	printw
	("\n    Each entry is as follows:  to hit / damage / number hits to kill\n");
	printw ("\n          monster     WC = 4         WC = 20        WC = 40");
	printw
	("\n---------------------------------------------------------------");
	for (i = 0; i <= MAXMONST + 8; i++) {
		hit = 2 * monster[i].armorclass + 2 * monster[i].level + 16;
		dam = 16 - cdesc[HARDGAME];
		lprintf ("\n%20s   %2d/%2d/%2d       %2d/%2d/%2d       %2d/%2d/%2d",
		         monster[i].name,
		         (int) (hit / 2), (int) max (0, dam + 2),
		         (int) (monster[i].hitpoints / (dam + 2) + 1),
		         (int) ((hit + 2) / 2), (int) max (0, dam + 10),
		         (int) (monster[i].hitpoints / (dam + 10) + 1),
		         (int) ((hit + 5) / 2), (int) max (0, dam + 20),
		         (int) (monster[i].hitpoints / (dam + 20) + 1));
	}
	printw ("\n\nHere's the list of available potions:\n\n");
	for (i = 0; i < MAXPOTION; i++)
		lprintf ("%20s\n", &potionname[i][1]);
	printw ("\n\nHere's the list of available scrolls:\n\n");
	for (i = 0; i < MAXSCROLL; i++)
		lprintf ("%20s\n", &scrollname[i][1]);
	printw ("\n\nHere's the spell list:\n\n");
	printw ("spell          name           description\n");
	printw
	("-------------------------------------------------------------------------------------------\n\n");
	for (j = 0; j < SPNUM; j++) {
		lprc (' ');
		printw (spelcode[j]);
		lprintf (" %21s  %s\n", spelname[j], speldescript[j]);
	}
	printw ("\n\nFor the cdesc[] array:\n");
	for (j = 0; j < 100; j += 10) {
		lprintf ("\nc[%2d] = ", (int) j);
		for (i = 0; i < 9; i++)
			lprintf ("%5d ", (int) cdesc[i + j]);
	}
	printw ("\n\nTest of random number generator ----------------");
	printw ("\n    for 25,000 calls divided into 16 slots\n\n");
	for (i = 0; i < 16; i++)
		rndcount[i] = 0;
	for (i = 0; i < 25000; i++)
		rndcount[rund (16)]++;
	for (i = 0; i < 16; i++) {
		lprintf ("  %5d", (int) rndcount[i]);
		if (i == 7)
			lprc ('\n');
	}
	printw ("\n\n");
	lwclose ();
	lcreat ((char *) 0);
	printw ("Done Diagnosing . . .");
	return (0);
}


/*
subroutine to draw the whole screen as the player knows it
*/
void
diagdrawscreen (void)
{
	int i, j, k;
	for (i = 0; i < MAXY; i++)
		/*  for the east west walls of this line    */
	{
		for (j = 0; j < MAXX; j++)
			if (k = mitem[j][i])
				lprc (monstnamelist[k]);
			else
				lprc (objnamelist[item[j][i]]);
		lprc ('\n');
	}
}
#endif

#ifdef ANTICHEAT

/*
* subroutine to not allow greedy cheaters
*/
static void
greedy (void)
{
#if WIZID
	if (wizard) {
		return;
	}
#endif
	printw
	("\n\nI am so sorry, but your character is a little TOO good!  Since this\n");
	printw
	("cannot normally happen from an honest game, I must assume that you cheated.\n");
	printw
	("In that you are GREEDY as well as a CHEATER, I cannot allow this game\n");
	printw ("to continue.\n");
	nap (5000);
	cdesc[GOLD] = cdesc[BANKACCOUNT] = 0;
	died (-267);
}


/*
* subroutine to not allow altered save files and terminate the attempted
* restart
*/
static void
fsorry (void)
{
	printw ("\nSorry, but your savefile has been altered.\n");
	printw ("However, seeing as I am a good sport, I will let you play.\n");
	printw ("Be advised though, you won't be placed on the normal scoreboard.");
	cheat = 1;
	nap (4000);
}


/*
* subroutine to not allow game if save file can't be deleted
*/
static void
fcheat (void)
{
#if WIZID
	if (wizard) {
		return;
	}
#endif
	printw
	("\nSorry, but your savefile can't be deleted.  This can only mean\n");
	printw
	("that you tried to CHEAT by protecting the directory the savefile\n");
	printw
	("is in.  Since this is unfair to the rest of the larn community, I\n");
	printw ("cannot let you play this game.\n");
	nap (5000);
	cdesc[GOLD] = cdesc[BANKACCOUNT] = 0;
	died (-268);
}

#endif
