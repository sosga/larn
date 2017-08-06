#include "../config/data.h"
#include "../lexical/tok.hpp"
#include "../dungeon/dungeon.hpp"
#include "../../includes/display.h"
#include "../../includes/global.h"
#include "help.hpp"
#include "inventory.hpp"
#include "../../includes/io.h"
#include "../../includes/main.h"
#include "../../includes/moreobj.h"
#include "../../includes/movem.h"
#include "../../includes/object.h"
#include "../player/regen.hpp"
#include "scores.hpp"
#include "../save/save.hpp"
#include "../../includes/spells.h"
#include "../player/hunger.hpp"
#include "funcs.hpp"

extern char viewflag;

void
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
            fl_termcap_cursor_position(1,24);
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
            fl_termcap_cursor_position(1,24);
            fl_display_message ( "\nPress any key to exit. " );
            scbr();
            ttgetch();
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
                fl_termcap_cursor_position(1,24);
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
            fl_termcap_cursor_position(1,24);
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
            fl_termcap_cursor_position(1,24);
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
            fl_termcap_cursor_position(1,24);
            fl_output_buffer_flush();
            save_mode = 1;
            save.savegame(savefilename);
            clearvt100();
            scbr();
    		exit(EXIT_SUCCESS);
            break;

        case 'T':
            y_larn_rep = 0;
            fl_termcap_cursor_position(1,24);

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

            fl_termcap_cursor_position(1,24);
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
            fl_termcap_cursor_position(1,24);
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
                fl_termcap_cursor_position(1,24);
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
                    if(strlen(scrollname[i]) > 2) {
                        object_identification[i][0] = OSCROLL;
                        object_argument[i][0] = i;
                    }

                for(i = MAXX - 1; i > MAXX - 1 - MAXPOTION; i--)

                    /* no null items */
                    if(strlen(potionname[i - MAXX + MAXPOTION]) > 2) {
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
    fl_regen_hp_and_spells();
	FLHunger.HungerLose();
}