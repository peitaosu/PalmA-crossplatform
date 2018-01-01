//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2018 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


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
