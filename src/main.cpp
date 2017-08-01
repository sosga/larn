#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <setjmp.h>
#include <curses.h>
#include "../src/config/larncons.h"
#include "../src/config/data.h"
#include "../src/templates/math.t.hpp"
#include "../src/terminal/term.hpp"
#include "lexical/tok.hpp"
#include "dungeon/dungeon.hpp"
#include "../includes/display.h"
#include "../includes/global.h"
#include "core/help.hpp"
#include "core/inventory.hpp"
#include "../includes/io.h"
#include "../includes/main.h"
#include "../includes/moreobj.h"
#include "../includes/movem.h"
#include "../includes/object.h"
#include "../includes/regen.h"
#include "core/scores.hpp"
#include "../includes/spells.h"
#include "../includes/spheres.h"
#include "save/save.hpp"
#include "strings/utf8.h"
#include "player/hunger.hpp"
#include "core/newgame.hpp"
#include "core/funcs.hpp"

using std::cout;

static void parse(void);
static void randmonst(void);
static void run(int);
static void wield(void);
static void ydhi(int);
static void ycwi(int);
static void wear(void);
static void dropobj(void);
static int floor_consume(int, const char *);
static void consume(int, const char *);
static int whatitem(const char *);

int dropflag =
    0;		/* if 1 then don't fl_look_for_an_object_and_give_options() next round */
int rmst = 80;			/*  random monster creation counter     */
int nomove =
    0;			/* if (nomove) then don't count next iteration as a
				   move */
static char viewflag =
    0;	/* if viewflag then we have done a 99 stay here
				   and don't showcell in the main loop */


signed int save_mode = 0;	/* 1 if doing a save game */
int restorflag = 0;

/*
************
MAIN PROGRAM
************
*/
int main()
{
    FILE *pFile;
    Load load;
	
	//Classes
	FLHunger FLHunger;
	FLGame FLGame;
    
    /*
     *  first task is to identify the player
     */
    /*init curses ~Gibbon */
    init_term();			/* setup the terminal (find out what type) for termcap */
    scbr();
    /*
     *  second task is to prepare the pathnames the player will need
     */
    /* Set up the input and output buffers.
     */
    lpbuf = (char *) operator new((5 * BUFBIG) >>
                                  2);	/* output buffer */
    inbuffer = (char *) operator new((5 * MAXIBUF) >>
                                     2);	/* output buffer */

    if((lpbuf == 0) || (inbuffer == 0)) {
        died(-285);     /* malloc() failure */
    }

    utf8ncpy(scorefile, SCORENAME, 50);	/* the larn scoreboard filename */
    utf8ncpy(logfile, LOGFNAME, 50);	/* larn activity logging filename */
    utf8ncpy(playerids, PLAYERIDS, 50);	/* the playerid data file name */
    utf8ncpy(savefilename, SAVEFILE, 50);
    /*
     *  now make scoreboard if it is not there (don't clear)
     */
    pFile = fopen(scorefile, "r");

    if(pFile == 0) {	/* not there */
        makeboard();
    }

    else {
        fclose(pFile);
    }

    /*
     *  He really wants to play, so malloc the memory for the dungeon.
     */
    const char *cell = (const char*) operator new((sizeof * cell) * (MAXLEVEL + MAXVLEVEL) *
                       MAXX * MAXY);

    if(cell == NULL) {
        /* malloc failure */
        died(-285);
    }

    lcreat((char *) 0);
    FLGame.NewGame();			/*  set the initial clock  */

    pFile = fopen(savefilename, "r");
    if(pFile != 0) {	/* restore game if need to */
        fclose(pFile);
        screen_clear();
        restorflag = 1;
        hitflag = 1;
        load.restoregame(savefilename);	/* restore last game    */
        remove(savefilename);
    }

    setupvt100();		/*  setup the terminal special mode             */

    if(cdesc[FL_HP] == 0) {	/* create new game */
        predostuff =
            1;		/* tell signals that we are in the welcome screen */
        welcome();		/* welcome the player to the game */
        makeplayer();		/*  make the character that will play           */
        newcavelevel(0);		/*  make the dungeon                            */
        /* Display their mail if they've just won the previous game
         */
        checkmail();
    }

    lprc(T_INIT);		/* Reinit the screen because of welcome and check mail
				 * having embedded escape sequences.*/
    drawscreen();		/*  show the initial dungeon */
    /* tell the trap functions that they must do a showplayer() from here on */
    predostuff = 2;
    y_larn_rep = hit2flag = 0;
    /*
     * init previous player position to be current position, so we don't
     * reveal any stuff on the screen prematurely.
     */
    oldx = playerx;
    oldy = playery;

    /* gtime = -1; */
    /* MAINLOOP
       find objects, move stuff, get commands, regenerate
     */
    for(;;) {
        if(dropflag == 0) {
            /* see if there is an object here.

               If in prompt mode, identify and prompt; else
               identify, never prompt.
             */
            fl_look_for_an_object_and_give_options(1, 0, 0);
        }

        else {
            dropflag = 0;		/* don't show it just dropped an object_identification */
        }

        /* handle global activity
           update game time, move spheres, move walls, move monsters
           all the stuff affected by FL_TIMESTOP and FL_HASTESELF
         */
        if(cdesc[FL_TIMESTOP] <= 0)
            if(cdesc[FL_HASTESELF] == 0 || (cdesc[FL_HASTESELF] & 1) == 0) {
                gtime++;
                movsphere();

                if(hitflag == 0) {
                    if(cdesc[FL_HASTEMONST]) {
                        movemonst();
                    }

                    movemonst();
                }
            }

        /* show stuff around the player
         */
        if(viewflag == 0) {
            showcell(playerx, playery);
        }

        else {
            viewflag = 0;
        }

        if(hit3flag)
#if defined WINDOWS
            lflushall();

#endif
#if defined NIX || NIX_LOCAL
        fflush(NULL);
#endif
        hitflag = hit3flag = 0;
        bot_linex();		/* update bottom line */
        /* get commands and make moves
         */
        nomove = 1;

        while(nomove) {
            if(hit3flag)
#if defined WINDOWS
                lflushall();

#endif
#if defined NIX || NIX_LOCAL
            fflush(NULL);
#endif
            nomove = 0;
            parse();
        }

        regen();			/*  regenerate hp and spells            */
		FLHunger.HungerLose();

        if(cdesc[FL_TIMESTOP] == 0)
            if(--rmst <= 0) {
                rmst = 120 - (level << 2);
                fillmonst(makemonst(level));
            }
    }
}

/*
* subroutine to randomly create monsters if needed
*/
static void
randmonst(void)
{
    /*  don't make monsters if time is stopped  */
    if(cdesc[FL_TIMESTOP]) {
        return;
    }

    if(--rmst <= 0) {
        rmst = 120 - (level << 2);
        fillmonst(makemonst(level));
    }
}

/*
* parse()
*
* get and execute a command
*/
static void
parse(void)
{
    FLCoreFuncs CoreFuncs;

    int i, j, k, flag;
    Save save;

    for(;;) {
        k = yylex();

        switch(k) {	/*  get the token from the input and switch on it   */
        case 'h':
            moveplayer(4);
            return;		/*  west        */

        case 'H':
            run(4);
            return;		/*  west        */

        case 'l':
            moveplayer(2);
            return;		/*  east        */

        case 'L':
            run(2);
            return;		/*  east        */

        case 'j':
            moveplayer(1);
            return;		/*  south       */

        case 'J':
            run(1);
            return;		/*  south       */

        case 'k':
            moveplayer(3);
            return;		/*  north       */

        case 'K':
            run(3);
            return;		/*  north       */

        case 'u':
            moveplayer(5);
            return;		/*  northeast   */

        case 'U':
            run(5);
            return;		/*  northeast   */

        case 'y':
            moveplayer(6);
            return;		/*  northwest   */

        case 'Y':
            run(6);
            return;		/*  northwest   */

        case 'n':
            moveplayer(7);
            return;		/*  southeast   */

        case 'N':
            run(7);
            return;		/*  southeast   */

        case 'b':
            moveplayer(8);
            return;		/*  southwest   */

        case 'B':
            run(8);
            return;		/*  southwest   */

        case '.':		/*  stay here       */
            if(y_larn_rep) {
                viewflag = 1;
            }

            return;

        case 'c':
            y_larn_rep = 0;
            cast();
            return;		/*  cast a spell    */

        case 'd':
            y_larn_rep = 0;

            if(cdesc[FL_TIMESTOP] == 0) {
                dropobj();
            }

            return;		/*  to drop an object   */

        case 'e':
            y_larn_rep = 0;

            if(cdesc[FL_TIMESTOP] == 0)
                if(!floor_consume(OCOOKIE, "eat")) {
                    consume(OCOOKIE, "eat");
					cdesc[HUNGER] += 10;
                }

            return;

        case 'g':
            y_larn_rep = 0;
            cursor(1,24);
            lprintf("\nThe stuff you are carrying presently weighs %d pounds",
                    (int) packweight());
            break;

        case 'i':		/* inventory */
            y_larn_rep = 0;
            nomove = 1;
            showstr(0);
            return;

        case 'p':		/* pray at an altar */
            y_larn_rep = 0;
            pray_at_altar();
            return;
        case '~':
            screen_clear();
            enable_scroll = 0;
            showscores (0);    /* if we updated the scoreboard */
            cursor(1,24);
            fl_display_message ( "\nPress any key to exit. " );
            scbr ();
            ttgetch ();
            enable_scroll = 1;
            drawscreen();
            return;

        case 'q':		/* quaff a potion */
            y_larn_rep = 0;

            if(cdesc[FL_TIMESTOP] == 0)
                if(!floor_consume(OPOTION, "quaff")) {
                    consume(OPOTION, "quaff");
                }

            return;

        case 'r':
            y_larn_rep = 0;

            if(cdesc[FL_BLINDCOUNT]) {
                cursor(1,24);
                fl_display_message("\nYou can't read anything when you're blind!");
            }

            else if(cdesc[FL_TIMESTOP] == 0)
                if(!floor_consume(OSCROLL, "read"))
                    if(!floor_consume(OBOOK, "read"))
                        if(!floor_consume(OPRAYERBOOK, "read")) {
                            consume(OSCROLL, "read");
                        }

            return;		/*  to read a scroll    */

        case 's':
            y_larn_rep = 0;
            sit_on_throne();
            return;

        case 't':		/* Tidy up at fountain */
            y_larn_rep = 0;
            wash_fountain();
            return;

        case 'v':
            y_larn_rep = 0;
            nomove = 1;
            cursor(1,24);
            lprintf("\nFreelarn, Version: %s",VERSION);

            if(wizard) {
                fl_display_message(" Wizard");
            }

            if(cheat) {
                fl_display_message(" Cheater");
            }

            return;

        case 'w':		/*  wield a weapon */
            y_larn_rep = 0;
            wield();
            return;

        case 'A':
            y_larn_rep = 0;
            desecrate_altar();
            return;

        case 'C':		/* Close something */
            y_larn_rep = 0;
            close_something();
            return;

        case 'D':		/* Drink at fountain */
            y_larn_rep = 0;
            drink_fountain();
            return;

        case '?':
            y_larn_rep = 0;
            display_help_text();
            nomove = 1;
            return;	/*give the help screen*/

        case 'E':		/* Enter a building */
            y_larn_rep = 0;
            enter();
            break;

        case 'I':		/*  list spells and scrolls */
            y_larn_rep = 0;
            seemagic(0);
            nomove = 1;
            return;

        case 'O':		/* Open something */
            y_larn_rep = 0;
            open_something();
            return;

        case 'P':
            cursor(1,24);
            y_larn_rep = 0;
            nomove = 1;

            if(outstanding_taxes > 0)
                lprintf("\nYou presently owe %d gp in taxes.",
                        (int) outstanding_taxes);

            else {
                fl_display_message("\nYou do not owe any taxes.");
            }

            return;

        case 'Q':		/*  quit        */
            y_larn_rep = 0;
            quit();
            nomove = 1;
            return;

        case 'R':		/* remove gems from a throne */
            y_larn_rep = 0;
            remove_gems();
            return;

        case 'S':
            /*
            Added the save stuff
            It's much better now!
            ~Gibbon
             */
            cursor(1,24);
            lflush();
            save_mode = 1;
            save.savegame(savefilename);
            clearvt100();
            scbr();
    		exit(EXIT_SUCCESS);
            break;

        case 'T':
            y_larn_rep = 0;
            cursor(1,24);

            if(cdesc[FL_SHIELD] != -1) {
                cdesc[FL_SHIELD] = -1;
                fl_display_message("\nYour shield is off");
                bottomline();
            }

            else if(cdesc[FL_WEAR] != -1) {
                cdesc[FL_WEAR] = -1;
                fl_display_message("\nYour armor is off");
                bottomline();
            }

            else {
                fl_display_message("\nYou aren't wearing anything");
            }

            return;

        case 'W':
            y_larn_rep = 0;
            wear();
            return;		/*  wear armor  */

        case 'Z':
            y_larn_rep = 0;

            if(cdesc[FL_LEVEL] > 9) {
                fl_teleport(1);
                return;
            }

            cursor(1,24);
            fl_display_message
            ("\nAs yet, you don't have enough experience to use teleportation");
            return;		/*  teleport yourself   */

        case ' ':
            y_larn_rep = 0;
            nomove = 1;
            return;

        case 'L' - 64:
            y_larn_rep = 0;
            drawscreen();
            nomove = 1;
            return;		/*  look        */

        case '<':		/* Go up stairs or vol shaft */
            y_larn_rep = 0;
            up_stairs();
            return;

        case '>':		/* Go down stairs or vol shaft */
            y_larn_rep = 0;
            down_stairs();
            return;

        case ',':		/* pick up an object_identification */
            y_larn_rep = 0;
            /* pickup, don't identify or prompt for action */
            fl_look_for_an_object_and_give_options(0, 1, 0);
            return;

        case ':':		/* look at object */
            y_larn_rep = 0;
            /* identify, don't pick up or prompt for action */
            fl_look_for_an_object_and_give_options(1, 0, 0);
            nomove = 1;		/* assumes look takes no time */
            return;

        case '/':		/* identify object/monster */
            specify_object();
            nomove = 1;
            y_larn_rep = 0;
            return;

        case '^':		/* identify traps */
            flag = y_larn_rep = 0;
            cursor(1,24);
            lprc('\n');

            for(j = playery - 1; j < playery + 2; j++) {
                if(j < 0) {
                    j = 0;
                }

                if(j >= MAXY) {
                    break;
                }

                for(i = playerx - 1; i < playerx + 2; i++) {
                    if(i < 0) {
                        i = 0;
                    }

                    if(i >= MAXX) {
                        break;
                    }

                    switch(object_identification[i][j]) {
                    case OTRAPDOOR:
                    case ODARTRAP:
                    case OTRAPARROW:
                    case OTELEPORTER:
                    case OPIT:
                        fl_display_message("\nIts ");
                        fl_display_message(objectname[object_identification[i][j]]);
                        flag++;
                    };
                }
            }

            if(flag == 0) {
                fl_display_message("\nNo traps are visible");
            }

            return;
            if (WIZID == true) {
            case '_':		/*  this is the fudge player password for wizard mode */
                y_larn_rep = 0;
                cursor(1,24);
                nomove = 1;
                wizard = 1;		/* disable to easily test win condition */
                scbr();		/* system("stty -echo cbreak"); */

                for(i = 0; i < 6; i++) {
                    cdesc[i] = 70;
                }

                iven[0] = iven[1] = 0;
                /* give the ring a little boost. ~Gibbon */
                take(OPROTRING, 51);
                /* lets nerf it a little bit.
                 * ~Gibbon */
                take(OGREATSWORD, 24);
                cdesc[FL_WIELD] = 1;
                cdesc[GREATSWORDDEATH] = 1;
                CoreFuncs.IncreaseExperience(6000000L);
                cdesc[FL_AWARENESS] += 25000;
                {
                    for(i = 0; i < MAXY; i++)
                        for(j = 0; j < MAXX; j++) {
                            been_here_before[j][i] = KNOWALL;
                        }

                    for(i = 0; i < SPNUM; i++) {
                        spelknow[i] = 1;
                    }

                    for(i = 0; i < MAXSCROLL; i++) {
                        scrollname[i][0] = ' ';
                    }

                    for(i = 0; i < MAXPOTION; i++) {
                        potionname[i][0] = ' ';
                    }
                }

                for(i = 0; i < MAXSCROLL; i++)

                    /* no null items */
                    if(utf8len(scrollname[i]) > 2) {
                        object_identification[i][0] = OSCROLL;
                        object_argument[i][0] = i;
                    }

                for(i = MAXX - 1; i > MAXX - 1 - MAXPOTION; i--)

                    /* no null items */
                    if(utf8len(potionname[i - MAXX + MAXPOTION]) > 2) {
                        object_identification[i][0] = OPOTION;
                        object_argument[i][0] = i - MAXX + MAXPOTION;
                    }

                for(i = 1; i < MAXY; i++) {
                    object_identification[0][i] = i;
                    object_argument[0][i] = 0;
                }

                for(i = MAXY; i < MAXY + MAXX; i++) {
                    object_identification[i - MAXY][MAXY - 1] = i;
                    object_argument[i - MAXY][MAXY - 1] = 0;
                }

                for(i = MAXX + MAXY; i < MAXOBJECT; i++) {
                    object_identification[MAXX - 1][i - MAXX - MAXY] = i;
                    object_argument[MAXX - 1][i - MAXX - MAXY] = 0;
                }

                cdesc[GOLD] += 250000;
                drawscreen();
                return;
            }
        };
    }
}



void
parse2(void)
{
	FLHunger FLHunger;
    /* move the monsters */
    if(cdesc[FL_HASTEMONST]) {
        movemonst();
    }

    movemonst();
    randmonst();
    regen();
	FLHunger.HungerLose();
}



static void
run(int sphere_direction)
{
	FLHunger FLHunger;
    int i;
    i = 1;

    while(i) {
        i = moveplayer(sphere_direction);

        if(i > 0) {
            if(cdesc[FL_HASTEMONST]) {
                movemonst();
            }

            movemonst();
            randmonst();
            regen();
			FLHunger.HungerLose();
        }

        if(hitflag) {
            i = 0;
        }

        if(i != 0) {
            showcell(playerx, playery);
        }
    }
}



/*
* function to wield a weapon
*/
static void
wield(void)
{
    int i;

    for(;;) {
        i = whatitem("wield (- for nothing)");

        if(i == '\33') {
            return;
        }

        if(i != '.') {
            if(i == '-') {
                cdesc[FL_WIELD] = -1;
                bottomline();
                return;
            }

            if(!i || i == '.') {
                continue;
            }

            if(iven[i - 'a'] == 0) {
                ydhi(i);
                return;
            }

            else if(iven[i - 'a'] == OPOTION) {
                ycwi(i);
                return;
            }

            else if(iven[i - 'a'] == OSCROLL) {
                ycwi(i);
                return;
            }

            else if(cdesc[FL_SHIELD] != -1
                    && iven[i - 'a'] == O2SWORD) {
                fl_display_message("\nBut one arm is busy with your shield!");
                return;
            }

            else if(cdesc[FL_SHIELD] != -1
                    && iven[i - 'a'] == OHSWORD) {
                fl_display_message("\nA longsword of Hymie cannot be used while a shield is equipped!");
                return;
            }

            else {
                cdesc[FL_WIELD] = i - 'a';

                if(iven[i - 'a'] == OGREATSWORD) {
                    cdesc[GREATSWORDDEATH] = 1;
                }

                else {
                    cdesc[GREATSWORDDEATH] = 0;
                }

                bottomline();
                return;
            }
        }
    }
}



/*
* common routine to say you don't have an object_identification
*/
static void
ydhi(int x)
{
    cursor(1,24);
    lprintf("\nYou don't have object_identification %c!", x);
}



/*
* common routine to say you can't wield an object_identification
*/
static void
ycwi(int x)
{
    cursor(1,24);
    lprintf("\nYou can't wield object_identification %c!", x);
}



/*
function to wear armor
*/
static void
wear(void)
{
    int i;

    for(;;) {
        if((i = whatitem("wear")) == '\33') {
            return;
        }

        if(i != '.' && i != '-') {
            if(i && i != '.')
                switch(iven[i - 'a']) {
                case 0:
                    ydhi(i);
                    return;

                case OLEATHER:
                case OCHAIN:
                case OPLATE:
                case ORING:
                case OSPLINT:
                case OPLATEARMOR:
                case OSTUDLEATHER:
                case OSSPLATE:
                    if(cdesc[FL_WEAR] != -1) {
                        fl_display_message("\nYou're already wearing some armor");
                        return;
                    }

                    cdesc[FL_WEAR] = i - 'a';
                    bottomline();
                    return;

                case OSHIELD:
                    if(cdesc[FL_SHIELD] != -1) {
                        fl_display_message("\nYou are already wearing a shield");
                        return;
                    }

                    if(iven[cdesc[FL_WIELD]] == O2SWORD) {
                        fl_display_message
                        ("\nYour hands are busy with the two handed sword!");
                        return;
                    }

                    if(iven[cdesc[FL_WIELD]] == OHSWORD) {
                        fl_display_message("\nYou are holding a longsword of Hymie!");
                        return;
                    }

                    cdesc[FL_SHIELD] = i - 'a';
                    bottomline();
                    return;

                default:
                    fl_display_message("\nYou can't wear that!");
                };
        }
    }
}




/*
function to drop an object
*/
static void
dropobj(void)
{
    int i;
    int *p;
    int amt;
    p = &object_identification[playerx][playery];

    for(;;) {
        if((i = whatitem("drop")) == '\33') {
            return;
        }
        if(i != '-') {
            if(i == '.') {	/* drop some gold */
                if(*p) {
                    lprintf("\nThere's something here already: %s",
                            objectname[object_identification[playerx][playery]]);
                    dropflag = 1;
                    return;
                }

                fl_display_message("\n\n");
                cl_dn(1, 23);
                fl_display_message("How much gold do you drop? ");

                if((amt = readnum((int) cdesc[GOLD])) == 0) {
                    return;
                }

                if(amt > cdesc[GOLD]) {
                    fl_display_message("\n");
                    fl_display_message("You don't have that much!");
                    return;
                }

                if(amt <= 32767) {
                    *p = OGOLDPILE;
                    i = amt;
                }

                else if(amt <= 327670L) {
                    *p = ODGOLD;
                    i = amt / 10;
                    amt = 10L * i;
                }

                else if(amt <= 3276700L) {
                    *p = OMAXGOLD;
                    i = amt / 100;
                    amt = 100L * i;
                }

                else if(amt <= 32767000L) {
                    *p = OKGOLD;
                    i = amt / 1000;
                    amt = 1000L * i;
                }

                else {
                    *p = OKGOLD;
                    i = 32767;
                    amt = 32767000L;
                }

                cdesc[GOLD] -= amt;
                lprintf("\nYou drop %d gold pieces", (int) amt);
                object_argument[playerx][playery] = i;
                bottomgold();
                been_here_before[playerx][playery] = 0;
                dropflag = 1;
                return;
            }

            if(i) {
                drop_object(i - 'a');
                return;
            }
        }
    }
}


static int
floor_consume(int search_item, const char *cons_verb)
{
    int i;
    char tempc;
    cursor(1,24);
    i = object_identification[playerx][playery];

    /* object_identification not there, quit
     */
    if(i != search_item) {
        return (0);
    }

    /* object_identification there.  does the player want to consume it?
     */
    lprintf("\nThere is %s", objectname[i]);

    if(i == OSCROLL)
        if(scrollname[object_argument[playerx][playery]][0]) {
            lprintf("%s", scrollname[object_argument[playerx][playery]]);
        }

    if(i == OPOTION)
        if(potionname[object_argument[playerx][playery]][0]) {
            lprintf("%s", potionname[object_argument[playerx][playery]]);
        }

    lprintf(" here.  Do you want to %s it?", cons_verb);

    if((tempc = getyn()) == 'n') {
        return (0);    /* object_identification there, not consumed */
    }

    else if(tempc != 'y') {
        fl_display_message(" aborted");
        return (-1);		/* abort */
    }

    /* consume the object_identification.
     */
    switch(i) {
    case OCOOKIE:
        forget();
        break;

    case OBOOK:
        readbook(object_argument[playerx][playery]);
        forget();
        break;

    case OPRAYERBOOK:
        readprayerbook(object_argument[playerx][playery]);
        forget();
        break;

    case OPOTION:
        fl_drink_potion(object_argument[playerx][playery], 1);
        forget();
        break;

    case OSCROLL:
        /* scrolls are tricky because of teleport.
         */
        i = object_argument[playerx][playery];
        been_here_before[playerx][playery] = 0;
        object_identification[playerx][playery] = object_argument[playerx][playery] = 0;
        fl_read_scroll(i);
        break;
    }

    return (1);
}



static void
consume(int search_item, const char *prompt)
{
    int i;

    for(;;) {
        if((i = whatitem(prompt)) == '\33') {
            return;
        }

        if(i != '.' && i != '-') {
            if(i && i != '.') {
                switch(iven[i - 'a']) {
                case OSCROLL:
                    if(search_item != OSCROLL) {
                        lprintf("\nYou can't %s that.", prompt);
                        return;
                    }

                    fl_read_scroll(ivenarg[i - 'a']);
                    break;

                case OBOOK:
                    if(search_item != OSCROLL) {
                        lprintf("\nYou can't %s that.", prompt);
                        return;
                    }

                    readbook(ivenarg[i - 'a']);
                    break;

                case OPRAYERBOOK:
                    if(search_item != OSCROLL) {
                        lprintf("\nYou can't %s that.", prompt);
                        return;
                    }

                    readprayerbook(ivenarg[i - 'a']);
                    break;

                case OCOOKIE:
                    if(search_item != OCOOKIE) {
                        lprintf("\nYou can't %s that.", prompt);
                        return;
                    }
                    break;

                case OPOTION:
                    if(search_item != OPOTION) {
                        lprintf("\nYou can't %s that.", prompt);
                        return;
                    }

                    fl_drink_potion(ivenarg[i - 'a'], 1);
                    break;

                case 0:
                    ydhi(i);
                    return;

                default:
                    lprintf("\nYou can't %s that.", prompt);
                    return;
                }

                iven[i - 'a'] = 0;
                return;
            }
        }
    }
}



/*
function to ask what player wants to do
*/
static int
whatitem(const char *str)
{
    int i = 0;
    cursor(1,24);
    lprintf("\nWhat do you want to %s? ", str);

    while(i > 'z' || (i < 'a' && i != '-' && i != '*' && i != '\33' && i != '.')) {
        i = ttgetch();
    }

    if(i == '\33') {
        fl_display_message(" aborted");
    }

    return (i);
}




/*
subroutine to get a number from the player
and allow * to mean return amt, else return the number entered
*/
int
readnum(int mx)
{
    int i;
    int amt = 0;
    sncbr();

    /* allow him to say * for all gold
     */
    if((i = ttgetch()) == '*') {
        amt = mx;
    }

    else

        /* read chars into buffer, deleting when requested */
        while(i != '\n') {
            if(i == '\033') {
                scbr();
                fl_display_message(" aborted");
                return (0);
            }

            if((i <= '9') && (i >= '0') && (amt < 999999999)) {
                amt = amt * 10 + i - '0';
            }

            if((i == '\010') || (i == '\177')) {
                amt = (int)(amt / 10);
            }

            i = ttgetch();
        }

    scbr();
    return (amt);
}
