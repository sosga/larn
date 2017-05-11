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
#include "../includes/bill.h"
#include "../includes/larncons.h"
#include "../includes/larndata.h"
#include "../includes/larnfunc.h"
#include "../includes/help.h"
#include "../includes/io.h"

static int junk_mail1(int);
static int junk_mail2(void);
static int junk_mail3(void);
static int junk_mail4(void);

/*I will start to change these mail messages.  Some of them can be changed,
	to make them more funny and interesting.  Right now, they are kinda boring and uninteresting.
-Gibbon
*/

/*
*  function to create the tax bill for the user
*/
static int
junk_mail1(int gold_count)
{
	resetscroll();
	screen_clear();
	lstandout("From:");
	lprcat("the LRS");
	lstandout("\nSubject:");
	lprcat("Income\n");
	lprcat("\nNews has come to us that you have indeed survived the caverns!");
	lprcat("\nCongratulations..");
	lprcat("\n\nWe received news that you have received");
	lprintf("\n%d Gold Coins.", gold_count);
	lprcat("\nAs you know, Larn is in economic trouble, thus we have prepared your tax bill.");
	lprintf("\nYou owe %d Gold Coins.", gold_count * TAXRATE);
	lprcat("\nPlease pay within 5 days or there will be consequences.\n");
	retcont();
	return(1);
}

static int
junk_mail2(void)
{
	resetscroll();
	screen_clear();
	lstandout("From:");
	lprcat("Monk Chilfred\n");
	lstandout("\nSubject:");
	lprcat("I have heard of your astounding adventure\n");
	lprcat("Would you like to take up my offer of another?\n");
	lprcat("I have news of a faraway land which has dangers and riches.\n");
	lprcat("We have news that our friend Bazar the Ronin has met his fate.\n");
	lprcat("We would be grateful if you could locate his bones,\n");
	lprcat("he went to the caves of n'hak where he was\n");
	lprcat("killed by a giant bat while sleeping..\n\n");
	lprcat("If you take us up on this offer, you will be rewarded.\n");
	retcont();
	return(1);
}

static int
junk_mail3(void)
{
	resetscroll();
	screen_clear();
	lstandout("From:");
	lprcat("Count Englewond of Chi\n");
	lstandout("\nSubject:");
	lprcat("!!!!\n");
	lprcat("\nI heard of your journey. Congratulations!");
	lprcat("\nWith several attempts I have yet to endure the");
	lprcat("caverns of Larn,\nand you, a nobody, makes the journey! From this time");
	lprcat("onward, bewarned that.....argh this darn keyboa..\n");
	retcont();
	return(1);
}

static int
junk_mail4(void)
{
	resetscroll();
	screen_clear();
	lstandout("From:");
	lprcat("Hakaz the Elderly\n");
	lstandout("\nSubject:");
	lprcat("Adomonis\n");
	lprcat("\nA terrible plague is sweeping Larn and people are dying a terrible death!");
	lprcat("\nI beseech thee to help save souls..");
	lprcat("\nI was an adventurer like yourself, saving the world from dangers.");
	lprcat("\nAny help you can spare will be appreciated.\n");
	retcont();
	return(1);
}

/* Entry point for junk mail. ~Gibbon */
void
larn_read_junk_mail(int gold_count)
{
	junk_mail1(gold_count);
	junk_mail2();
	junk_mail3();
	junk_mail4();
}
