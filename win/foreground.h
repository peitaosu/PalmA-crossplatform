#ifndef PALMA_FOREGROUND_H
#define PALMA_FOREGROUND_H
#include <string>
#include <Windows.h>
#include "tlhelp32.h"

class Foreground
{
    public:
        Foreground();
        std::string getForegroundProcessName();
        std::string getForegroundWindowName();
        LPWSTR GetProcessNameById(DWORD ProcessID);
};

#endif // PALMA_FOREGROUND_H
