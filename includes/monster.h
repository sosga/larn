void checkloss ( int );
void createitem ( int, int );
void createmonster ( int );
void dropgold ( int );

class HitMonster
{
public:
    int hitm(int,int,int);
    int hpoints;
    int amt2;
    int lastx;
    int lasty;
};

void hitmonster ( int, int );
void hitplayer ( int, int );
int newobject ( int, int * );
void something ( int );
int vxy ( int *, int * );

inline const int ARMORTYPES = 6;
