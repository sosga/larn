#define _POSIX_C_SOURCE 200809L
#ifdef WIN32
#include <windows.h>
#elif _POSIX_C_SOURCE
#include <time.h>
#else
#include <unistd.h>
#endif

void nap(int milliseconds)
{
#ifdef WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE
    struct timespec tmsc;
    tmsc.tv_sec = milliseconds / 1000;
    tmsc.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&tmsc, NULL);
#else
    usleep(milliseconds * 1000);
#endif
}
