/* help.c */
#include <curses.h>
#include <cstdio>
#include <cstdlib>
#include "includes/larncons.h"
#include "includes/larndata.h"
#include "includes/larnfunc.h"
#include "includes/ansiterm.h"
#include "includes/display.h"
#include "includes/help.h"
#include "includes/io.h"
#include "includes/sysdep.h"

/* I coded a nice line-reader for this.  Unfortunately it's very hard to get a good
 *  cursor and text position when reading a file from inside a curses program.
 *  so I did this instead, I actually prefer it. -Gibbon

 */

int
display_help_text(void)
{
	screen_clear();
	lstandout("Help File for Larn\n");
	printw("b  move southwest          B  run southwest            A  desecrate an altar\n");
	printw("c  cast a spell            C  close a door             Z  teleport yourself	\n");
	printw("d  drop an item            D  drink at a fountain      <  go up stairs or	\n");
	printw("e  eat something           E  enter a store, dungeon      volcanic shaft	\n");
	printw("g  get present pack weight                             >  go down stairs or	\n");
	printw("h  move left               H  run left                    volcanic shaft	\n");
	printw("i  inventory your pockets  I  list all known items							\n");
	printw("j  move down               J  run down                 ^  identify a trap	\n");
	printw("k  move up                 K  run up                   ,  pick up item		\n");
	printw("l  move right              L  run right                :  look at object you\n");
	printw("n  move southeast          N  run southeast               are standing on	\n");
	printw("                           O  open a door or chest     .  stay here			\n");
	printw("p  pray at an altar        P  give tax status          m  move without picking\n");
	printw("q  quaff a potion          Q  quit the game (suicide)     up an object		\n");
	printw("r  read a scroll           R  remove gems from throne  /  identify objects in\n");
	printw("s  sit on a throne         S  save the game               the game			\n");
	printw("t  tidy up at a fountain   T  take off armor  								\n");
	printw("u  move northeast          U  run northeast									\n");
	printw("v  print program version													\n");
	printw("w  wield a weapon          W  wear armor 									\n");
	printw("y  move northwest          Y  run northwest            ^L redraw the screen	\n");
	retcont();
	screen_clear();
	lstandout("Special Notes\n");
	printw("When dropping gold, if you type '*' as your amount, all your gold gets dropped.\n");
	printw("In general, typing in '*' means all of what your interested in.  This is true\n");
	printw("when visiting the bank, or when contributing at altars.\n\n");
	printw("When in the store, trading post, school, or home, an <escape> will get you out.\n\n");
	printw("When casting a spell, if you need a list of spells you can cast, type 'I' as\n");
	printw("the first letter of your spell.  The available list of spells will be shown,\n");
	printw("after which you may enter the spell code.  This only works on the 1st letter\n");
	printw("of the spell you are casting.\n\n");
	printw("When an inventory list is on the screen from a drop, quaff, read, or similar\n");
	printw("command, you can type the letter of the object that you wish to act apon,\n");
	printw("without having to type a space to get back to the prompt.\n\n");
	printw("If NumLock is off, the Keypad functions in the obvious way for movement. Hold\n");
	printw("Shift when pressing any direction on the Keypad to run in that direction. The\n");
	printw("5 key on the Keypad is the same as \"stay here\", which really means to\n");
	printw("skip your turn.");
	retcont();
	screen_clear();
	lstandout("Explanation of the Larn scoreboard facility\n");
	printw("Larn supports TWO scoreboards, one for winners, and one for deceased\n");
	printw("characters.  Each player (by the name entered when you start the game)\n");
	printw("is allowed one slot on each scoreboard, if the score is in the top ten for\n");
	printw("that scoreboard.  This design helps insure that frequent players of Larn\n");
	printw("do not hog the scoreboard, and gives more players a chance for glory.\n\n");
	printw("Level of difficulty is also noted on the scoreboards, and this takes precedence\n");
	printw("over score for determining what entry is on the scoreboard.  For example:\n");
	printw("if \"Yar, the Bug Slayer\" has a score of 128003 on the scoreboard at diff 0,\n");
	printw("then a game at diff 1 and a score of 4112 would replace the previous\n");
	printw("entry on the scoreboard.  Note that when a player dies, the inventory is\n");
	printw("stored in the scoreboard so that everyone can see what items the player had\n");
	printw("at the time of death.");
	return_to_game();
	drawscreen();
	setscroll();
	return(0);
}

/*
 * function to display the welcome message and background
 */
void
welcome (void)
{
	printw("Welcome to the game of Larn.\n\n  At this moment, you face a great problem.");
	printw("\nYour daughter has contracted a strange disease, and none of your");
	printw("\nhome remedies seem to have any effect. You sense that she is in");
	printw("\nmortal danger, and you must try to save her.");
	printw("\n\n  Time ago you heard of a land of great danger and opportunity.");
	printw("\nPerhaps here is the solution you need.");
	printw("\n\n  It has been said that there once was a great magician who called himself");
	printw("\nPolinneaus.  Many years ago after having many miraculous successes,");
	printw("\nPolinneaus retired to the caverns of Larn, where he devoted most of his");
	printw("\ntime to the creation of magic. Rumors have it that one day Polinneaus set");
	printw("\nout to dispel an attacking army in a forest some distance to the north.");
	printw("\nIt is believed that here he met his demise. The caverns of Larn it is");
	printw("\nthought, must be magnificent in design and contain much magic and treasure.");
	printw("\nOne option you have is to undertake a journey into these caverns in hope");
	printw("\nof finding a cure.");
	printw("\n");
	printw("\n\nGood Luck!  You're going to need it!");
	/* press return to continue */
	retcont();
}

/*
 * function to say press return to continue and reset scroll when done
 */
void
retcont(void)
{
	cursor(1, 24);
	printw("Press ");
	lstandout("return");
	printw(" to continue: ");
	while (ttgetch() != '\n');
	setscroll ();
}

/* Added for places where it points you back to the game. -Gibbon */
void
return_to_game(void)
{
	cursor(1, 24);
	printw("Press ");
	lstandout("return");
	printw(" to go back to the game: ");
	while (ttgetch() != '\n');
	setscroll();
}
