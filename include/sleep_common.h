#ifndef SLEEP_COMMON
#define SLEEP_COMMON

#include "config/platform.h"

#ifdef PLATFORM_WIN32
    #include <Windows.h>
    #define sleep_ms(x) Sleep(x)
#elif defined PLATFORM_LINUX
    #include <unistd.h>
    #define sleep_ms(x) usleep(x * 1000)
#endif

#endif
