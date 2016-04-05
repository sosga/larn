#include "larnfunc.h"
#include "io.h"
#include "sysdep.h"

#if defined WINDOWS
#include <windows.h>
#endif

#if defined LINUX || DARWIN || BSD
#include <time.h>
#endif

void nap(int milliseconds)
{
#ifdef WINDOWS
    Sleep(milliseconds);
#endif

#if defined LINUX || DARWIN || BSD
    struct timespec tc;
    tc.tv_sec = milliseconds / 1000;
    tc.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&tc, NULL);
#endif
}
