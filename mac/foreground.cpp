//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2017 Tony Su All Rights Reserved //
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
    //get pid from "lsappinfo front", use awk to find the process id
    std::string process_id = exec("lsappinfo info -only pid `lsappinfo front` | awk -F= '{print $2}'");
    //get bundle path from "lsappinfo front", use awk to find the bundle name
    return exec("lsappinfo info -only bundlepath `lsappinfo front` | awk -F '/' '{print $NF}'");
}

/*
 * Function: Get Foreground Window Name
 * Input: NONE
 * Output: String Window Name
 */
std::string Foreground::getForegroundWindowName(){
    //get pid from "lsappinfo front", use awk to find the process id
    std::string process_id = exec("lsappinfo info -only pid `lsappinfo front` | awk -F= '{print $2}'");
    //get LSDisplayName from "lsappinfo front", use awk to find the process name
    return exec("lsappinfo info -only name `lsappinfo front` | awk -F= '{print $2}'");
}
