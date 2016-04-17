#ifndef FOREGROUND_H
#define FOREGROUND_H
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

#endif // FOREGROUND_H
