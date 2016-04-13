/* help.c */
#include <stdio.h>
#include "includes/larncons.h"
#include "includes/larndata.h"
#include "includes/larnfunc.h"
#include "includes/ansiterm.h"
#include "includes/display.h"
#include "includes/help.h"
#include "includes/io.h"
#include "includes/sysdep.h"

/*
 * function to display the welcome message and background
 */
void
welcome (void)
{
	lprcat("Welcome to the game of Larn.\n\n  At this moment, you face a great problem.");
	lprcat("\nYour daughter has contracted a strange disease, and none of your");
	lprcat("\nhome remedies seem to have any effect. You sense that she is in");
	lprcat("\nmortal danger, and you must try to save her.");
	lprcat("\n\n  Time ago you heard of a land of great danger and opportunity.");
	lprcat("\nPerhaps here is the solution you need.");
	lprcat("\n\n  It has been said that there once was a great magician who called himself");
	lprcat("\nPolinneaus.  Many years ago after having many miraculous successes,");
	lprcat("\nPolinneaus retired to the caverns of Larn, where he devoted most of his");
	lprcat("\ntime to the creation of magic. Rumors have it that one day Polinneaus set");
	lprcat("\nout to dispel an attacking army in a forest some distance to the north.");
	lprcat("\nIt is believed that here he met his demise. The caverns of Larn it is");
	lprcat("\nthought, must be magnificent in design and contain much magic and treasure.");
	lprcat("\nOne option you have is to undertake a journey into these caverns in hope");
	lprcat("\nof finding a cure.");
	lprcat("\n");
	lprcat("\n\nGood Luck!  You're going to need it!");
	
	/* press return to continue */
	retcont();
}



/*
 * function to say press return to continue and reset scroll when done
 */
void
retcont (void)
{
  cursor (1, 24);
  lprcat ("Press ");
  lstandout ("return");
  lprcat (" to continue: ");
  while (ttgetch () != '\n');
  setscroll ();
}
