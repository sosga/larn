/*  Copyright 2017 Gibbon aka 'atsb'
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

#include "FLHelp.hpp"
#include "FLInventory.hpp"
#include "FLTermCapIO.hpp"
#include "FLScores.hpp"
#include "FLFuncs.hpp"
#include "FLStore.hpp"
#include "../config/FLIncludeData.hpp"
#include "../lexical/FLTok.hpp"
#include "../dungeon/FLDungeon.hpp"
#include "../terminal/FLTerminal.hpp"
#include "../player/FLPlayerActions.hpp"
#include "../player/FLRegenerate.hpp"
#include "../save/FLSave.hpp"
#include "../player/FLHunger.hpp"
#include "../terminal/FLGetChar.hpp"
#include "../json/FLKeysConfig.hpp"

extern char viewflag;
	
void
parse(void) {
	FLCoreFuncs CoreFuncs;
	FLStore Store;
	Save save;
	int i, j, k, flag;
	
	//Reading JSON
	fl_json_config_move_reader.parse(fl_json_file_move, fl_json_config_move_value);
	
	//JSON movement variables linking to the configuration file
	int WEST = fl_json_config_move_value["MOVE-WEST"].asInt();
	int EAST = fl_json_config_move_value["MOVE-EAST"].asInt();
	int SOUTH = fl_json_config_move_value["MOVE-SOUTH"].asInt();
	int NORTH = fl_json_config_move_value["MOVE-NORTH"].asInt();
	int NORTHEAST = fl_json_config_move_value["MOVE-NORTHEAST"].asInt();
	int NORTHWEST = fl_json_config_move_value["MOVE-NORTHWEST"].asInt();
	int SOUTHEAST = fl_json_config_move_value["MOVE-SOUTHEAST"].asInt();
	int SOUTHWEST = fl_json_config_move_value["MOVE-SOUTHWEST"].asInt();
	int MOVESTAY = fl_json_config_move_value["MOVE-STAY"].asInt();
	
	//JSON lower-case variables linking to the configuration file
	int KEY_LOWERCASE_C = fl_json_config_move_value["KEY-LOWERCASE-C"].asInt();
	int KEY_LOWERCASE_D = fl_json_config_move_value["KEY-LOWERCASE-D"].asInt();
	int KEY_LOWERCASE_E = fl_json_config_move_value["KEY-LOWERCASE-E"].asInt();
	int KEY_LOWERCASE_I = fl_json_config_move_value["KEY-LOWERCASE-I"].asInt();
	int KEY_LOWERCASE_P = fl_json_config_move_value["KEY-LOWERCASE-P"].asInt();
	int KEY_HOME_TILDE = fl_json_config_move_value["KEY-HOME-TILDE"].asInt();
	int KEY_LOWERCASE_Q = fl_json_config_move_value["KEY-LOWERCASE-Q"].asInt();
	int KEY_LOWERCASE_R = fl_json_config_move_value["KEY-LOWERCASE-R"].asInt();
	int KEY_LOWERCASE_S = fl_json_config_move_value["KEY-LOWERCASE-S"].asInt();
	int KEY_LOWERCASE_T = fl_json_config_move_value["KEY-LOWERCASE-T"].asInt();
	int KEY_LOWERCASE_V = fl_json_config_move_value["KEY-LOWERCASE-V"].asInt();
	int KEY_LOWERCASE_W = fl_json_config_move_value["KEY-LOWERCASE-W"].asInt();
	int KEY_LOWERCASE_G = fl_json_config_move_value["KEY-LOWERCASE-G"].asInt();
	
	//JSON upper-case and misc variables linking to the configuration file
	int KEY_UPPERCASE_A = fl_json_config_move_value["KEY-UPPERCASE-A"].asInt();
	int KEY_UPPERCASE_C = fl_json_config_move_value["KEY-UPPERCASE-C"].asInt();
	int KEY_UPPERCASE_D = fl_json_config_move_value["KEY-UPPERCASE-D"].asInt();
	int KEY_QUESTION_MARK = fl_json_config_move_value["KEY-QUESTION-MARK"].asInt();
	int KEY_UPPERCASE_E = fl_json_config_move_value["KEY-UPPERCASE-E"].asInt();
	int KEY_UPPERCASE_I = fl_json_config_move_value["KEY-UPPERCASE-I"].asInt();
	int KEY_UPPERCASE_O = fl_json_config_move_value["KEY-UPPERCASE-O"].asInt();
	int KEY_UPPERCASE_P = fl_json_config_move_value["KEY-UPPERCASE-P"].asInt();
	int KEY_UPPERCASE_Q = fl_json_config_move_value["KEY-UPPERCASE-Q"].asInt();
	int KEY_UPPERCASE_R = fl_json_config_move_value["KEY-UPPERCASE-R"].asInt();
	int KEY_UPPERCASE_S = fl_json_config_move_value["KEY-UPPERCASE-S"].asInt();
	int KEY_UPPERCASE_T = fl_json_config_move_value["KEY-UPPERCASE-T"].asInt();
	int KEY_UPPERCASE_W = fl_json_config_move_value["KEY-UPPERCASE-W"].asInt();
	int KEY_UPPERCASE_Z = fl_json_config_move_value["KEY-UPPERCASE-Z"].asInt();
	int KEY_EMPTY_WHITESPACE = fl_json_config_move_value["KEY-EMPTY-WHITESPACE"].asInt();
	int KEY_UPPERCASE_L = fl_json_config_move_value["KEY-UPPERCASE-L"].asInt();
	int KEY_LEFT_ARROW = fl_json_config_move_value["KEY-LEFT-ARROW"].asInt();
	int KEY_RIGHT_ARROW = fl_json_config_move_value["KEY-RIGHT-ARROW"].asInt();
	int KEY_COMMA = fl_json_config_move_value["KEY-COMMA"].asInt();
	int KEY_COLON = fl_json_config_move_value["KEY-COLON"].asInt();
	int KEY_FORWARD_SLASH = fl_json_config_move_value["KEY-FORWARD-SLASH"].asInt();
	int KEY_UP_ARROW = fl_json_config_move_value["KEY-UP-ARROW"].asInt();
	int KEY_UNDERSCORE = fl_json_config_move_value["KEY-UNDERSCORE"].asInt();
	
	for (;;) {
		k = yylex();
			if (k == WEST)
			{
				moveplayer(4);
				return;
			}
			if (k == EAST)
			{
				moveplayer(2);
				return;
			}
			if (k == SOUTH)
			{
				moveplayer(1);
				return;
			}
			if (k == NORTH)
			{
				moveplayer(3);
				return;
			}
			if (k == NORTHEAST)
			{
				moveplayer(5);
				return;
			}
			if (k == NORTHWEST)
			{
				moveplayer(6);
				return;
			}
			if (k == SOUTHEAST)
			{
				moveplayer(7);
				return;
			}
			if (k == SOUTHWEST)
			{
				moveplayer(8);
				return;
			}
			if (k == MOVESTAY)
			{
				if (y_larn_rep) {
					viewflag = 1;
				}
				return;
			}
			if (k == KEY_LOWERCASE_C)
			{
				y_larn_rep = 0;
				fl_cast_a_spell();
				return;		/*  cast a spell    */
			}
			if (k == KEY_LOWERCASE_D)
			{
				y_larn_rep = 0;
				if (cdesc[FL_TIMESTOP] == 0)
				{
					dropobj();
				}
				return;		/*  to drop an object   */
			}
			if (k == KEY_LOWERCASE_E)
			{
				y_larn_rep = 0;
				if (cdesc[FL_TIMESTOP] == 0)
					if (!floor_consume(OCOOKIE, "eat"))
					{
						consume(OCOOKIE, "eat");
						if (cdesc[FL_HUNGER] < cdesc[FL_HUNGERMAX])
						{
							cdesc[FL_HUNGER] += 10;
						} else {
							fl_display_message("\nIt has no affect on your appetite.");
						}
					}
				return;
			}
			if (k == KEY_LOWERCASE_G)
			{
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				lprintf("\nThe stuff you are carrying presently weighs %d pounds", packweight());
				return;
			}
			if (k == KEY_LOWERCASE_I)
			{
				y_larn_rep = 0;
				nomove = 1;
				showstr(0);
				return;
			}
			if (k == KEY_LOWERCASE_P)
			{
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (object_identification[player_horizontal_position][player_vertical_position] != OALTAR)
				{
					fl_display_message("\nI see no altar to pray at here!");
				} else {
					fl_a_give_donation();
				}
				prayed = 1;
				return;
			}
			if (k == KEY_HOME_TILDE)
			{
				fl_clear_and_reset_screen();
				enable_scroll = 0;
				showscores(0);     /* if we updated the scoreboard */
				fl_termcap_cursor_position(1, 24);
				fl_display_message("\nPress any key to exit. ");
				get_final_char_keyboard = ansiterm_getch;
				ttgetch();
				enable_scroll = 1;
				drawscreen();
				return;
			}
			if (k == KEY_LOWERCASE_Q)
			{
				y_larn_rep = 0;
				if (cdesc[FL_TIMESTOP] == 0)
					if (!floor_consume(OPOTION, "quaff"))
					{
						consume(OPOTION, "quaff");
					}
				return;
			}
			if (k == KEY_LOWERCASE_R)
			{
				y_larn_rep = 0;
				if (cdesc[FL_BLINDCOUNT])
				{
					fl_termcap_cursor_position(1, 24);
					fl_display_message("\nYou can't read anything when you're blind!");
				} else if (cdesc[FL_TIMESTOP] == 0)
					if (!floor_consume(OSCROLL, "read"))
						if (!floor_consume(OBOOK, "read"))
							if (!floor_consume(OPRAYERBOOK, "read"))
							{
								consume(OSCROLL, "read");
							}
				return;
			}
			if (k == KEY_LOWERCASE_S)
			{
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (object_identification[player_horizontal_position][player_vertical_position] == OTHRONE)
				{
					fl_a_sit_on_throne(0);
				} else if ((object_identification[player_horizontal_position][player_vertical_position] == OTHRONE2) ||
				   (object_identification[player_horizontal_position][player_vertical_position] == ODEADTHRONE))
				{
					fl_a_sit_on_throne(1);
				} else {
					fl_display_message("\nI see no throne to sit on here!");
				}
				return;
			}
			if (k == KEY_LOWERCASE_T)
			{
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (object_identification[player_horizontal_position][player_vertical_position] == ODEADFOUNTAIN)
				{
					fl_display_message("\nThere is no water to wash in!");
				} else if (object_identification[player_horizontal_position][player_vertical_position] != OFOUNTAIN)
				{
					fl_display_message("\nI see no fountain to wash at here!");
				} else {
					fl_a_wash_in_fountain();
				}
				return;
			}
			if (k == KEY_LOWERCASE_V)
			{
				y_larn_rep = 0;
				nomove = 1;
				fl_termcap_cursor_position(1, 24);
				lprintf("\nVersion: %s", FL_BUILD_VERSION);
				lprintf("\n%s", FL_APACHE2_NOTICE);
				if (wizard) {
					fl_display_message(" Wizard");
				}
				if (cheat) {
					fl_display_message(" Cheater");
				}
				return;
			}
			if (k == KEY_LOWERCASE_W)
			{
				y_larn_rep = 0;
				wield();
				return;
			}
			if (k == KEY_UPPERCASE_A)
			{
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (object_identification[player_horizontal_position][player_vertical_position] == OALTAR)
				{
					fl_a_desecrate_an_altar();
				} else {
					fl_display_message("\nI see no altar to desecrate here!");
				}
				return;
			}
			if (k == KEY_UPPERCASE_C)
			{
				y_larn_rep = 0;
				fl_close_an_object();
				return;
			}
			if (k == KEY_UPPERCASE_D)
			{
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (object_identification[player_horizontal_position][player_vertical_position] == ODEADFOUNTAIN)
				{
					fl_display_message("\nThere is no water to drink!");
				} else if (object_identification[player_horizontal_position][player_vertical_position] != OFOUNTAIN)
				{
					fl_display_message("\nI see no fountain to drink from here!");
				} else {
					fl_a_drink_from_fountain();
				}
				return;
			}
			if (k == KEY_QUESTION_MARK)
			{
				y_larn_rep = 0;
				display_help_text();
				nomove = 1;
				return;
			}
			if (k == KEY_UPPERCASE_E)
			{
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				switch (object_identification[player_horizontal_position][player_vertical_position])
				{
					case OBANK:
						banktitle("    Welcome to the First National Bank of Larn.");
						break;
					case OBANK2:
						banktitle("Welcome to the 5th level branch office of the First National Bank of Larn.");
						/*  because we state the level in the title, clear the '?' in the
						    level display at the bottom, if the user teleported.
						*/
						cdesc[FL_TELEPORT_FLAG] = 0;
						break;
					case ODNDSTORE:
						Store.DNDStore();
						break;
					case OENTRANCE:
						/*  place player in front of entrance on level 1.  fl_generate_a_new_dungeon_level()
						    prevents player from landing on a monster/object.
						*/
						player_horizontal_position = 33;
						player_vertical_position = FL_MAX_VERTICAL_POSITION - 2;
						fl_generate_a_new_dungeon_level(1);
						been_here_before[33][FL_MAX_VERTICAL_POSITION - 1] = KNOWALL;
						monster_identification[33][FL_MAX_VERTICAL_POSITION - 1] = 0;
						draws(0, FL_MAX_HORIZONTAL_POSITION, 0, FL_MAX_VERTICAL_POSITION);
						showcell(player_horizontal_position, player_vertical_position);	/* to show around player */
						bot_linex();
						break;
					case OTRADEPOST:
						Store.TradingPost();
						break;
					case OLRS:
						Store.TaxOffice();
						break;
					case OHOME:
						fl_player_home();
						break;
					case FL_OBJECT_TEMPLE_IN:
						fl_a_enter_the_temple();
						break;
					case FL_OBJECT_TEMPLE_OUT:
						fl_a_exit_the_temple();
						break;
					default:
						fl_display_message("\nThere is no place to enter here!\n");
						break;
				}
				return;
			}
			if (k == KEY_UPPERCASE_I)
			{
				y_larn_rep = 0;
				seemagic(0);
				nomove = 1;
				return;
			}
			if (k == KEY_UPPERCASE_O)
			{
				y_larn_rep = 0;
				fl_open_an_object();
				return;
			}
			if (k == KEY_UPPERCASE_P)
			{
				fl_termcap_cursor_position(1, 24);
				y_larn_rep = 0;
				nomove = 1;
				if (outstanding_taxes > 0)
					lprintf("\nYou presently owe %d gp in taxes.", outstanding_taxes);
				else
				{
					fl_display_message("\nYou do not owe any taxes.");
				}
				return;
			}
			if (k == KEY_UPPERCASE_Q)
			{
				y_larn_rep = 0;
				fl_exit_the_game();
				nomove = 1;
				return;
			}
			if (k == KEY_UPPERCASE_R)
			{
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (object_identification[player_horizontal_position][player_vertical_position] == ODEADTHRONE)
				{
					fl_display_message("\nThere are no gems to remove!");
				} else if (object_identification[player_horizontal_position][player_vertical_position] == OTHRONE)
				{
					fl_a_remove_the_gems(0);
				} else if (object_identification[player_horizontal_position][player_vertical_position] == OTHRONE2)
				{
					fl_a_remove_the_gems(1);
				} else {
					fl_display_message("\nI see no throne here to remove gems from!");
				}
				return;
			}
			if (k == KEY_UPPERCASE_S)
			{
				/*
				    Added the save stuff
				    It's much better now!
				    ~Gibbon
				*/
				fl_termcap_cursor_position(1, 24);
				fl_output_buffer_flush();
				save_mode = 1;
				save.savegame(savefilename);
				fl_clear_terminal();
				get_final_char_keyboard = ansiterm_getch;
				exit(EXIT_SUCCESS);
				return;
			}
			if (k == KEY_UPPERCASE_T)
			{
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (cdesc[FL_SHIELD] != -1)
				{
					cdesc[FL_SHIELD] = -1;
					fl_display_message("\nYour shield is off");
					bottomline();
				} else if (cdesc[FL_WEAR] != -1)
				{
					cdesc[FL_WEAR] = -1;
					fl_display_message("\nYour armor is off");
					bottomline();
				} else {
					fl_display_message("\nYou aren't wearing anything");
				}
				return;
			}
			if (k == KEY_UPPERCASE_W)
			{
				y_larn_rep = 0;
				wear();
				return;		/*  wear armor  */
			}
			if (k == KEY_UPPERCASE_Z)
			{
				y_larn_rep = 0;
				if (cdesc[FL_LEVEL] > 9)
				{
					fl_teleport(1);
					return;
				}
				fl_termcap_cursor_position(1, 24);
				fl_display_message("\nAs yet, you don't have enough experience to use teleportation");
				return;		/*  teleport yourself   */
			}
			if (k == KEY_EMPTY_WHITESPACE)
			{
				y_larn_rep = 0;
				nomove = 1;
				return;
			}
			if (k == KEY_UPPERCASE_L - 64)
			{
				y_larn_rep = 0;
				drawscreen();
				nomove = 1;
				return;		/*  look        */
			}
			if (k == KEY_LEFT_ARROW)
			{
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (object_identification[player_horizontal_position][player_vertical_position] == OSTAIRSDOWN)
				{
					fl_display_message("\nThe stairs don't go up!");
				} else if (object_identification[player_horizontal_position][player_vertical_position] != OSTAIRSUP)
				{
					fl_display_message("\nI see no way to go up here!");
				} else {
					fl_a_travel_up_stairs();
				}
				return;
			}
			if (k == KEY_RIGHT_ARROW)
			{
				y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				if (object_identification[player_horizontal_position][player_vertical_position] == OSTAIRSUP)
				{
					fl_display_message("\nThe stairs don't go down!");
				} else if (object_identification[player_horizontal_position][player_vertical_position] != OSTAIRSDOWN)
				{
					fl_display_message("\nI see no way to go down here!");
				} else {
					fl_a_travel_down_stairs();
				}
				return;
			}
			if (k == KEY_COMMA)
			{
				y_larn_rep = 0;
				/* pickup, don't identify or prompt for action */
				fl_look_for_an_object_and_give_options(0, 1, 0);
				return;
			}
			if (k == KEY_COLON)
			{
				y_larn_rep = 0;
				/* identify, don't pick up or prompt for action */
				fl_look_for_an_object_and_give_options(1, 0, 0);
				nomove = 1;		/* assumes look takes no time */
				return;
			}
			if (k == KEY_FORWARD_SLASH)
			{
				fl_termcap_cursor_position(1, 24);
				fl_specify_an_object_by_charater();
				nomove = 1;
				y_larn_rep = 0;
				return;
			}
			if (k == KEY_UP_ARROW)
			{
				flag = y_larn_rep = 0;
				fl_termcap_cursor_position(1, 24);
				fl_output_single_byte_to_output_buffer('\n');
				for (j = player_vertical_position - 1; j < player_vertical_position + 2; j++)
				{
					if (j < 0)
					{
						j = 0;
					}
					if (j >= FL_MAX_VERTICAL_POSITION)
					{
						break;
					}
					for (i = player_horizontal_position - 1; i < player_horizontal_position + 2; i++)
					{
						if (i < 0) {
							i = 0;
						}
						if (i >= FL_MAX_HORIZONTAL_POSITION)
						{
							break;
						}
						switch (object_identification[i][j])
						{
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
				if (flag == 0)
				{
					fl_display_message("\nNo traps are visible");
				}
				return;
			}
			if (FL_WIZARD_MODE_ENABLED == true)
			{
				if (k == KEY_UNDERSCORE)
				{
					y_larn_rep = 0;
					fl_termcap_cursor_position(1, 24);
					nomove = 1;
					wizard = 1;		/* disable to easily test win condition */
					get_final_char_keyboard = ansiterm_getch;		/* system("stty -echo cbreak"); */
					for (i = 0; i < 6; i++) {
						cdesc[i] = 70;
					}
					iven[0] = iven[1] = 0;
					/* give the ring a little boost. ~Gibbon */
					take(OPROTRING, 51);
					/*  lets nerf it a little bit.
					    ~Gibbon */
					take(OGREATSWORD, 24);
					cdesc[FL_WIELD] = 1;
					cdesc[FL_GREAT_SWORD_OF_DEATH] = 1;
					CoreFuncs.IncreaseExperience(6000000L);
					cdesc[FL_AWARENESS] += 25000;
					{
						for (i = 0; i < FL_MAX_VERTICAL_POSITION; i++)
							for (j = 0; j < FL_MAX_HORIZONTAL_POSITION; j++)
							{
								been_here_before[j][i] = KNOWALL;
							}
						for (i = 0; i < FL_MAX_SPELLS_IN_GAME; i++)
						{
							spelknow[i] = 1;
						}
						for (i = 0; i < MAXSCROLL; i++)
						{
							scrollname[i][0] = ' ';
						}
						for (i = 0; i < MAXPOTION; i++)
						{
							potionname[i][0] = ' ';
						}
					}
					for (i = 0; i < MAXSCROLL; i++)
						/* no null items */
						if (strlen(scrollname[i]) > 2) {
							object_identification[i][0] = OSCROLL;
							object_argument[i][0] = i;
						}
					for (i = FL_MAX_HORIZONTAL_POSITION - 1; i > FL_MAX_HORIZONTAL_POSITION - 1 - MAXPOTION; i--)
						/* no null items */
						if (strlen(potionname[i - FL_MAX_HORIZONTAL_POSITION + MAXPOTION]) > 2)
						{
							object_identification[i][0] = OPOTION;
							object_argument[i][0] = i - FL_MAX_HORIZONTAL_POSITION + MAXPOTION;
						}
					for (i = 1; i < FL_MAX_VERTICAL_POSITION; i++)
					{
						object_identification[0][i] = i;
						object_argument[0][i] = 0;
					}
					for (i = FL_MAX_VERTICAL_POSITION; i < FL_MAX_VERTICAL_POSITION + FL_MAX_HORIZONTAL_POSITION; i++)
					{
						object_identification[i - FL_MAX_VERTICAL_POSITION][FL_MAX_VERTICAL_POSITION - 1] = i;
						object_argument[i - FL_MAX_VERTICAL_POSITION][FL_MAX_VERTICAL_POSITION - 1] = 0;
					}
					for (i = FL_MAX_HORIZONTAL_POSITION + FL_MAX_VERTICAL_POSITION; i < MAXOBJECT; i++)
					{
						object_identification[FL_MAX_HORIZONTAL_POSITION - 1][i - FL_MAX_HORIZONTAL_POSITION - FL_MAX_VERTICAL_POSITION] = i;
						object_argument[FL_MAX_HORIZONTAL_POSITION - 1][i - FL_MAX_HORIZONTAL_POSITION - FL_MAX_VERTICAL_POSITION] = 0;
					}
					cdesc[FL_GOLD] += 250000;
					drawscreen();
					return;
				}
			}
			return;
		};
	}

void
parse2(void) {
	FLHunger FLHunger;
	/* move the monsters */
	if (cdesc[FL_HASTEMONST]) {
		fl_move_a_monster();
	}
	fl_move_a_monster();
	if (cdesc[FL_TIMESTOP]) {
		return;
	}
	if (--random_monster_creation <= 0) {
		random_monster_creation = 120 - (level << 2);
		fl_fill_dungeon_with_monsters(fl_create_monster(level));
	}
	fl_regen_hp_and_spells();
	FLHunger.HungerLose();
}