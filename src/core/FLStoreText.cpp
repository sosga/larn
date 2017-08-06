#include "store.hpp"
#include "../../includes/main.h"
#include "../config/data.h"
#include "../../includes/io.h"

/* new function for displaying gold in inventory inside trading posts.
 * part of feature request by hymie0. ~Gibbon */
int FLStore::AmountGoldTrad(void) {
	lprintf("You have");
    lprintf(" %-6d ",cdesc[GOLD]);
    lprintf("gold pieces.");
    return(0);
}
/*Fix for bug #23 ~Gibbon*/
int FLStore::LRSWelcomeText(void) {
    screen_clear();
    enable_scroll = 0;
    lprintf("Welcome to the Larn Revenue Service\n");
    return(0);
}
