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
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include "../config/larncons.h"
#include "../config/data.h"
#include "../templates/math.t.hpp"
#include "../terminal/term.hpp"
#include "message_log.hpp"
#include "../../includes/display.h"
#include "../../includes/help.h"
#include "../../includes/io.h"
#include "../../includes/sysdep.h"

/* I coded a nice line-reader for this.  Unfortunately it's very hard to get a good
 *  cursor and text position when reading a file from inside a curses program.
 *  so I did this instead, I actually prefer it. -Gibbon

 */

int
display_message_log ( void )
{
    screen_clear();
    lstandout ( "Message File for Larn\n" );
   // lopen("data/message.log");
    return 0;
}
