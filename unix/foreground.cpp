#include "foreground.h"
#include <iostream>
#include <string>
#include <stdio.h>
Foreground::Foreground()
{

}

std::string Foreground::exec(char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
    }
    pclose(pipe);
    return result;
}

std::string Foreground::getForegroundProcessName(){
    std::string process_id = exec("xprop -root | grep _NET_ACTIVE_WINDOW\(WINDOW\) | awk -F# '{gsub(/ /,,);print ;}'");
    return exec("xprop -id " + process_id + " | grep WM_CLASS\(STRING\) | awk -F\" '{print $2}'");
}

std::string Foreground::getForegroundWindowName(){
    std::string process_id = exec("xprop -root | grep _NET_ACTIVE_WINDOW\(WINDOW\) | awk -F# '{gsub(/ /,,);print ;}'");
    return exec("xprop -id " + process_id + " | grep WM_NAME\(STRING\) | awk -F\" '{print $2}'");
}
