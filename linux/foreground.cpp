//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2018 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


#include "foreground.h"
#include <iostream>
#include <string>
#include <stdio.h>
Foreground::Foreground()
{

}

/*
 * Function: Execute Command
 * Input: Char* Command
 * Output: String Command Return
 */
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

/*
 * Function: Get Foreground Process Name
 * Input: NONE
 * Output: String Process Name
 */
std::string Foreground::getForegroundProcessName(){
    //get _NET_ACTIVE_WINDOW(WINDOW) from "xprop -root", use awk to find the process id
    std::string process_id = exec("xprop -root | grep _NET_ACTIVE_WINDOW\(WINDOW\) | awk -F# '{gsub(/ /,,);print ;}'");
    //get WM_CLASS(STRING) from "xprop -id", use awk to find the process name
    return exec("xprop -id " + process_id + " | grep WM_CLASS\(STRING\) | awk -F\" '{print $2}'");
}

/*
 * Function: Get Foreground Window Name
 * Input: NONE
 * Output: String Window Name
 */
std::string Foreground::getForegroundWindowName(){
    //get _NET_ACTIVE_WINDOW(WINDOW) from "xprop -root", use awk to find the process id
    std::string process_id = exec("xprop -root | grep _NET_ACTIVE_WINDOW\(WINDOW\) | awk -F# '{gsub(/ /,,);print ;}'");
    //get WM_NAME(STRING) from "xprop -id", use awk to find the window name
    return exec("xprop -id " + process_id + " | grep WM_NAME\(STRING\) | awk -F\" '{print $2}'");
}
