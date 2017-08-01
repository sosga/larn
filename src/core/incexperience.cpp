#include "funcs.hpp"
#include "../templates/math.t.hpp"
#include "../config/larncons.h"
#include "../config/data.h"
#include "../../includes/display.h"
#include "../../includes/io.h"

/*
* raiseexperience(x)
*
* subroutine to increase experience points
*/
void FLCoreFuncs::IncreaseExperience (long x) {
    int i, tmp;
    i = cdesc[FL_LEVEL];
    cdesc[EXPERIENCE] += x;

    while(cdesc[EXPERIENCE] >= skill[cdesc[FL_LEVEL]] && (cdesc[FL_LEVEL] < MAXPLEVEL)) {
        tmp = (cdesc[CONSTITUTION]) >> 1;
        cdesc[FL_LEVEL]++;
        FL_RAISEMAXHEALTH(TRnd(3) + TRnd((tmp > 0) ? tmp : 1));
        FL_RAISEMAXSPELLS(TRund(3));

        if (cdesc[FL_LEVEL] < 7) {
            FL_RAISEMAXHEALTH(cdesc[CONSTITUTION] >> 2);
        }
	}
    if (cdesc[FL_LEVEL] != i) {
        cursor(1,24);
        fl_display_message("\nWelcome to level %d",cdesc[FL_LEVEL]);	/* if we changed levels */
    }
    bottomline();
}