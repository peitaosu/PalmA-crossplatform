#ifndef PALMA_FOREGROUND_H
#define PALMA_FOREGROUND_H

#include <string>

class Foreground
{
    public:
        Foreground();
        std::string Foreground::exec(char* cmd);
        std::string getForegroundProcessName();
        std::string getForegroundWindowName();
};

#endif // PALMA_FOREGROUND_H
