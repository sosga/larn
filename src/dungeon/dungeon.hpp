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

#include <curses.h>

void makeplayer(void);
void newcavelevel(int);
void eat(int,int);
int fillmonst(int);
void checkgen(void);
void sethp(int);
void makeobject(int);
void makemaze(int);
int newsphere(int, int, int, int);
int fl_remove_sphere_of_annihilation(int, int);
void fl_effect_of_sphere_detonation ( int x, int y );
void movsphere(void);
int fl_sphere_boom(int, int);

/* maximum number of spheres movsphere can handle */
inline const int SPHMAX = 20;