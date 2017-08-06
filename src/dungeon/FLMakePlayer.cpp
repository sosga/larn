/* Copyright 2017 Gibbon aka 'atsb'

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

#include "../../includes/main.h"
#include "../config/data.h"
#include "../templates/math.t.hpp"
#include "../terminal/term.hpp"
#include "../../includes/global.h"
#include "../../includes/io.h"
#include "../../includes/monster.h"
#include "../core/scores.hpp"
#include "../core/inventory.hpp"
#include "../save/save.hpp"
#include "../lexical/tok.hpp"
#include "dungeon.hpp"
#include "../json/flconfig.hpp"

using namespace std;

/*
makeplayer()

subroutine to create the player and the players attributes
this is called at the beginning of a game and at no other time
*/

void
makeplayer(void)
{
    int i;
    scbr();
    screen_clear();
    fl_json_config_reader.parse(fl_json_file, fl_json_config_value);
    cdesc[FL_HPMAX] = cdesc[FL_HP] = fl_json_config_value["HP"].asUInt();
    cdesc[HUNGERMAX] = cdesc[HUNGER] = fl_json_config_value["HGR"].asInt();
    /*  player starts at level one          */
    cdesc[FL_LEVEL] = fl_json_config_value["LEVEL"].asUInt();
    /*  total # spells starts off as 3  */
    cdesc[FL_SPELLMAX] = cdesc[FL_SPELLS] = fl_json_config_value["SPELLS"].asUInt();
    /* start regeneration correctly */
    cdesc[FL_REGENCOUNTER] = fl_json_config_value["REGENCOUNTER"].asUInt();
    cdesc[FL_ECOUNTER] = fl_json_config_value["ECOUNTER"].asUInt();
    cdesc[FL_SHIELD] = cdesc[FL_WEAR] = cdesc[FL_WIELD] = -1;

    for(i = 0; i < MAXINVEN; i++) {
        iven[i] = 0;
    }

    /* he knows protection, magic missile */
    spelknow[0] = spelknow[1] = 1;

    iven[0] = fl_json_config_value["STARTING-ARMOR"].asInt();
    iven[1] = fl_json_config_value["STARTING-WEAPON"].asInt();
    iven[2] = 0;
    ivenarg[1] = ivenarg[0] = cdesc[FL_WEAR] = 0;
    cdesc[FL_WIELD] = 1;

    playerx = TRnd(MAXX - 2);
    playery = TRnd(MAXY - 2);
    regen_bottom = 1;

    /* make the attributes, ie str, int, etc. */
    for(i = 0; i < 6; i++) {
        cdesc[i] = MAXINVEN;
    }

    fl_recalculate_armor_class();
    screen_clear();
    enter_name();
}
