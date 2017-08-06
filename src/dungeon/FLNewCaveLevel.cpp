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
#include "../save/save.hpp"
#include "dungeon.hpp"

/*
newcavelevel(level)
int level;

function to enter a new level.  This routine must be called anytime the
player changes levels.  If that level is unknown it will be created.
A new set of monsters will be created for a new level, and existing
levels will get a few more monsters.
Note that it is here we remove genocided monsters from the present level.
*/

void
newcavelevel(int x)
{
    int i, j;

    level = x;	/* get the new level and put in working storage*/
    for (i = 0; i < MAXY ; i++) {
        for (j = 0 ; j < MAXX ; j++) {
            been_here_before[j][i] = 0;
            monster_identification[j][i] = 0;
        }
    }
    makemaze(x);
    makeobject(x);
    beenhere[x]=1;   /* first time here */
    sethp(1);

    if (wizard || x==0)
        for (j=0; j<MAXY; j++)
            for (i=0; i<MAXX; i++)
                been_here_before[i][j]=1;

    checkgen();	/* wipe out any genocided monsters */
}