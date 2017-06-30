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

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <setjmp.h>
#include "../config/larncons.h"
#include "../config/data.h"
#include "../templates/math.t.hpp"
#include "../terminal/term.hpp"
#include "../../includes/global.h"
#include "../../includes/io.h"
#include "../../includes/monster.h"
#include "../../includes/scores.h"
#include "../save/save.hpp"
#include "dungeon.hpp"

/*
subroutine to put monsters into an empty room without walls or other
monsters
*/
int
fillmonst(int what)
{
    int x, y, trys;

    for(trys = 5; trys > 0; --trys) {	/* max # of creation attempts */
        x = TRnd(MAXX - 2);
        y = TRnd(MAXY - 2);

        if((item[x][y] == 0) && (mitem[x][y] == 0)
                && ((playerx != x) || (playery != y))) {
            mitem[x][y] = what;
            know[x][y] &= ~KNOWHERE;
            hitp[x][y] = monster[what].hitpoints;
            return (0);
        }
    }

    return -1;			/* creation failure */
}
