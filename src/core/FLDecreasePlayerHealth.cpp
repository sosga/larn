#include "funcs.hpp"
#include "../../includes/main.h"
#include "../config/data.h"

/*
* loselevel()
*
* subroutine to lower the players character level by one
*/
void FLCoreFuncs::DecreasePlayerLevel(void) {
	FLCoreFuncs CoreFuncs;
	if (cdesc[FL_LEVEL] > 1) {
		CoreFuncs.DecreaseExperience((cdesc[EXPERIENCE] - skill[cdesc[FL_LEVEL] - 1] + 1));
	}
}