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

/*
  newsphere(x,y,sphere_direction,sphere_duration)  Function to create a new sphere of annihilation
  fl_remove_sphere_of_annihilation(x,y)      Function to delete a sphere of annihilation from list
  fl_effect_of_sphere_detonation(x,y)       Function to perform the effects of a sphere detonation
  movsphere()        Function to look for and move spheres of annihilation
*/

#include "../../includes/main.h"
#include "../config/data.h"
#include "../templates/math.t.hpp"
#include "../../includes/display.h"
#include "../../includes/global.h"
#include "../core/help.hpp"
#include "../../includes/io.h"
#include "../../includes/monster.h"
#include "../core/scores.hpp"
#include "dungeon.hpp"
#include "../core/sysdep.hpp"

int
fl_sphere_boom(int x, int y)
{
	fl_effect_of_sphere_detonation ( x, y );
	fl_remove_sphere_of_annihilation ( x, y );
	return ( cdesc[SPHCAST] );
}

/*
 *  newsphere(x,y,sphere_direction,sphere_duration)  Function to create a new sphere of annihilation
 *      int x,y,sphere_direction,sphere_duration;
 *
 *  Enter with the coordinates of the sphere in x,y
 *    the direction (0-8 diroffx format) in sphere_direction, and the lifespan of the
 *    sphere in sphere_duration (in turns)
 *  Returns the number of spheres currently in existence
 */
int
newsphere ( int x, int y, int sphere_direction, int life )
{
    int m;
    struct sphere *sp;

    if (((sp = (struct sphere *)operator new(sizeof(struct sphere)))) == 0) {
        return ( cdesc[SPHCAST] );
    }

    if ( sphere_direction >= 9 ) {
        sphere_direction = 0;  /* no movement if direction not found */
    }

    if ( level == 0 ) {
        vxy ( &x, &y );  /* don't go out of bounds */
    }

    else {
        if ( x < 1 ) {
            x = 1;
        }

        if ( x >= MAXX - 1 ) {
            x = MAXX - 2;
        }

        if ( y < 1 ) {
            y = 1;
        }

        if ( y >= MAXY - 1 ) {
            y = MAXY - 2;
        }
    }

    if ( ( m = monster_identification[x][y] ) >= DEMONLORD +
            4 ) {	/* demons dispel spheres */
        fl_show_designated_cell_only ( x, y );		/* show the demon (ha ha) */
        fl_termcap_cursor_position(1,24);
        lprintf ( "\nThe %s dispels the sphere!", monster[m].name );
        fl_remove_sphere_of_annihilation ( x, y );		/* remove any spheres that are here */
        return ( cdesc[SPHCAST] );
    }

    if ( m == DISENCHANTRESS ) {	/* disenchantress cancels spheres */
        fl_termcap_cursor_position(1,24);
        lprintf ( "\nThe %s causes cancellation of the sphere!",
                  monster[m].name );
	}

    if ( cdesc[FL_CANCELLATION] ) {	/* cancellation cancels spheres */
        fl_termcap_cursor_position(1,24);
        fl_display_message
        ( "\nAs the cancellation takes effect, you hear a great earth shaking blast!" );
        fl_sphere_boom(x,y);
    }

    if ( object_identification[x][y] == FL_OBJECT_SPHERE_OF_ANNIHILATION) {	/* collision of spheres detonates spheres */
        fl_termcap_cursor_position(1,24);
        fl_display_message
        ( "\nTwo spheres of annihilation collide! You hear a great earth shaking blast!" );
        fl_remove_sphere_of_annihilation ( x, y );
        fl_sphere_boom(x,y);
    }

    if ( playerx == x && playery == y ) {	/* collision of sphere and player! */
        fl_termcap_cursor_position(1,24);
        fl_display_message ( "\nYou have been enveloped by the zone of nothingness!\n" );
        fl_remove_sphere_of_annihilation ( x, y );		/* remove any spheres that are here */
        fl_wait ( FL_WAIT_DURATION );
        fl_player_death ( 258 );
    }

    object_identification[x][y] = FL_OBJECT_SPHERE_OF_ANNIHILATION;
    monster_identification[x][y] = 0;
    been_here_before[x][y] = 1;
    fl_show_designated_cell_only ( x, y );		/* show the new sphere */
    sp->x = x;
    sp->y = y;
    sp->lev = level;
    sp->sphere_direction = sphere_direction;
    sp->sphere_duration = life;
    sp->p = 0;

    if ( spheres == 0 ) {
        spheres = sp;  /* if first node in the sphere list */
    }

    else {			/* add sphere to beginning of linked list */
        sp->p = spheres;
        spheres = sp;
    }

    return ( ++cdesc[SPHCAST] );	/* one more sphere in the world */
}


/*
 *  fl_remove_sphere_of_annihilation(x,y)       Function to delete a sphere of annihilation from list
 *      int x,y;
 *
 *  Enter with the coordinates of the sphere (on current level)
 *  Returns the number of spheres currently in existence
 */
int
fl_remove_sphere_of_annihilation ( int x, int y )
{
    struct sphere *sp, *sp2 = 0;

    for ( sp = spheres; sp; sp2 = sp, sp = sp->p )
        if ( level == sp->lev )	/* is sphere on this level? */
            if ( ( x == sp->x )
                    && ( y == sp->y ) ) {	/* locate sphere at this location */
                object_identification[x][y] = monster_identification[x][y] = 0;
                been_here_before[x][y] = 1;
                fl_show_designated_cell_only ( x, y );	/* show the now missing sphere */
                --cdesc[SPHCAST];

                if ( sp == spheres ) {
                    sp2 = sp;
                    spheres = sp->p;
                    free ( ( char * ) sp2 );
                }

                else {
                    sp2->p = sp->p;
                    free ( ( char * ) sp );
                }

                break;
            }

    /* return number of spheres in the world */
    return cdesc[SPHCAST];
}

/*
 *  movsphere()     Function to look for and move spheres of annihilation
 *
 *  This function works on the sphere linked list, first duplicating the list
 *  (the act of moving changes the list), then processing each sphere in order
 *  to move it.  They eat anything in their way, including stairs, volcanic
 *  shafts, potions, etc, except for upper level demons, who can dispel
 *  spheres.
 *  No value is returned.
 */

void
movsphere ( void )
{
    int x, y;
    struct sphere *sp, *sp2;
    struct sphere sph[SPHMAX];

    /* first duplicate sphere list */
    for ( sp = 0, x = 0, sp2 = spheres; sp2;
            sp2 = sp2->p )	/* look through sphere list */
        if ( sp2->lev == level ) {	/* only if this level */
            sph[x] = *sp2;
            sph[x++].p = 0;		/* copy the struct */

            if ( x > 1 ) {
                sph[x - 2].p = &sph[x - 1];  /* link pointers */
            }
        }

    if ( x ) {
        sp = sph;  /* if any spheres, point to them */
    }

    else {
        return;  /* no spheres */
    }

    for ( sp = sph; sp;
            sp = sp->p ) {	/* look through sphere list */
        x = sp->x;
        y = sp->y;

        if ( object_identification[x][y] != FL_OBJECT_SPHERE_OF_ANNIHILATION ) {
            continue;  /* not really there */
        }

        if ( -- ( sp->sphere_duration ) < 0 ) {	/* has sphere run out of gas? */
            fl_remove_sphere_of_annihilation ( x, y );	/* delete sphere */
            continue;
        }
        /* Nothing special and then advance the sphere.
         * ~Gibbon
         */
        if (TRnd(TMathMax(7, 5))) {
            /* sphere changes direction */
            sp->sphere_direction = TRnd(8);
        }
        sp = sp2;
    }
}
