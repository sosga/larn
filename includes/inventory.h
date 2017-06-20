void init_inventory ( void );

int showstr (char);

int showwear ( void );

int showwield ( void );

int showread ( void );

int showeat ( void );

int showquaff ( void );

void show1 ( int );

int show3 ( int );

int take ( int, int );

int drop_object ( int );

int pocketfull ( void );

/* Allow only 12 items (a to l) in the player's inventory */
inline const int MAXINVEN = 12;

/* The starting limit to the number of items the player can carry.
The limit should probably be based on player strength and the
weight of the items.
*/
inline const int MIN_LIMIT = 10;

/* define a sentinel to place at the end of the sorted inventory.
(speeds up display reads )
*/
inline const int END_SENTINEL = 255;
