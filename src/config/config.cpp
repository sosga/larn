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

/*
 *  config.cpp	This defines the installation dependent variables.
 *              Some strings are modified later.  ANSI C would
 *              allow compile time string concatenation, we must
 *              do runtime concatenation, in main.
 */
#include "larncons.h"
#include "data.h"
#include "../templates/math.t.hpp"

char scorefile[PATHLEN];
char logfile[PATHLEN];
char mazefile[PATHLEN];
char playerids[PATHLEN];
char savefilename[PATHLEN];
