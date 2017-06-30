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
* function to eat away a filled in maze
*/
void
eat (int xx, int yy)
{
    int dir, fl_eat_try;

    dir = TRnd (4);

    fl_eat_try = 2;

    while (fl_eat_try) {
        switch (dir) {
        case 1:
            if (xx <= 2)
                break;		/*  west    */
            if ((item[xx - 1][yy] != OWALL) || (item[xx - 2][yy] != OWALL))
                break;
            item[xx - 1][yy] = item[xx - 2][yy] = 0;
            eat (xx - 2, yy);
            break;

        case 2:
            if (xx >= MAXX - 3)
                break;		/*  east    */
            if ((item[xx + 1][yy] != OWALL) || (item[xx + 2][yy] != OWALL))
                break;
            item[xx + 1][yy] = item[xx + 2][yy] = 0;
            eat (xx + 2, yy);
            break;

        case 3:
            if (yy <= 2)
                break;		/*  south   */
            if ((item[xx][yy - 1] != OWALL) || (item[xx][yy - 2] != OWALL))
                break;
            item[xx][yy - 1] = item[xx][yy - 2] = 0;
            eat (xx, yy - 2);
            break;

        case 4:
            if (yy >= MAXY - 3)
                break;		/*  north   */
            if ((item[xx][yy + 1] != OWALL) || (item[xx][yy + 2] != OWALL))
                break;
            item[xx][yy + 1] = item[xx][yy + 2] = 0;
            eat (xx, yy + 2);
            break;
        };
        if (++dir > 4) {
            dir = 1;
            --fl_eat_try;
        }
    }
}