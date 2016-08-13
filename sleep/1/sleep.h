#ifndef SLEEP_H_INCLUDED
#define SLEEP_H_INCLUDED

#ifdef _WINDOWS
#include <windows.h>
#define sleep(x) Sleep(x)
#else
#include <unistd.h>
#define sleep(x) usleep((x)*1000)
#endif

#endif