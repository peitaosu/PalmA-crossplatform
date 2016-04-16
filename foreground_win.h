#ifndef FOREGROUND_H
#define FOREGROUND_H
#include <string>

class Foreground
{
    public:
        Foreground();
        std::string getForegroundProcessName();
        std::string getForegroundWindowName();
};

#endif // FOREGROUND_H
