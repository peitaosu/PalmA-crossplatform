//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2018 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


#include "operation.h"
#include "error_code.h"
#include <QUrl>
#include <QDesktopServices>
#include <QProcess>

#define SIDE_UP 1
#define SIDE_DOWN 2
#define SIDE_LEFT 3
#define SIDE_RIGHT 4

Operation::Operation(QObject *parent) : QObject(parent){
    event = new Event();
}

/*
 * Operation: Mouse Select
 * Input: double x, double y, bool press
 * Return: NONE
 */
void Operation::mouseSelect(double x, double y, bool press){
    event->mouseMove(x, y);
    if(press == true){
        event->mousePress(1);
    }else{
        event->mouseRelease(1);
    }
}

/*
 * Operation: Mouse Select (update)
 * Input: double x, double y
 * Return: NONE
 */
void Operation::mouseSelect(double x, double y){
    event->mouseMove(x, y);
}

/*
 * Operation: Mouse Roll
 * Input: int distance
 * Return: NONE
 */
void Operation::mouseRoll(int distance){
    event->mouseRoll(distance);
}

/*
 * Operation: Open Something, double click application/folder
 * Input: double x, double y
 * Return: NONE
 */
void Operation::openSomething(double x, double y){
    event->mouseMove(x, y);
    event->mouseDClick(1);
}

/*
 * Operation: Execute Program
 * Input: QString Program Path
 * Return: NONE
 */
void Operation::execProgram(QString program_path){
    QProcess::startDetached(program_path ,QStringList());
}

//Window
/*
 * Operation: Swipe Window
 * Input: int count, count > 0
 * Return: if input less than 1, return INPUT_ERROR, defined in error_code.h
 */
int Operation::swipeWindow(int count){
    if(count < 1){
        return INPUT_ERROR;
    }
    event->keyboardPress("ALT");
    while(count > 0){
        event->keyboardType("TAB");
    }
    event->keyboardRelease("ALT");
    return 0;
}

/*
 * Operation: Move Window Side
 * Input: int side, SIDE_UP, SIDE_DOWN, SIDE_LEFT, SIDE_RIGHT defined in the file head
 * Return: if input not in the list, return INPUT_ERROR, defined in error_code.h
 */
int Operation::moveWindow(int side){
    switch(side){
        case SIDE_UP:
            event->keyboardMType("LWIN+UP");
            break;
        case SIDE_DOWN:
            event->keyboardMType("LWIN+DOWN");
            break;
        case SIDE_LEFT:
            event->keyboardMType("LWIN+LEFT");
            break;
        case SIDE_RIGHT:
            event->keyboardMType("LWIN+RIGHT");
            break;
        default:
            return INPUT_ERROR;
    }
    return 0;
}

/*
 * Operation: Show Desktop
 * Input: NONE
 * Return: NONE
 */
void Operation::showDesktop(){
    event->keyboardMType("LWIN+D");
}

/*
 * Operation: Open Explorer
 * Input: NONE
 * Return: NONE
 */
void Operation::openExplorer(){
    event->keyboardMType("LWIN+E");
}

/*
 * Operation: Open Browser
 * Input: QString url
 * Return: NONE
 */
void Operation::openBrowser(QString url){
    QUrl url_go(url);
    QDesktopServices::openUrl(url_go);
}

/*
 * Operation: Goto
 * Input: NONE
 * Return: NONE
 */
void Operation::goTo(){
    event->keyboardType("ENTER");
}

/*
 * Operation: Go Back
 * Input: NONE
 * Return: NONE
 */
void Operation::goBack(){
    event->keyboardType("BACKSPACE");
}

/*
 * Operation: Refresh the Page
 * Input: NONE
 * Return: NONE
 */
void Operation::goRefresh(){
    event->keyboardType("F5");
}

/*
 * Operation: Swipe Browser Tab
 * Input: int count, count > 0
 * Return: if count<1, return INPUT_ERROR, defined in error_code.h
 */
int Operation::swipeBrowserTab(int count){
    if(count < 1){
        return INPUT_ERROR;
    }
    event->keyboardMType("CTRL+TAB");
    return 0;
}

/*
 * Operation: Lock Screen
 * Input: NONE
 * Return: NONE
 */
void Operation::lockscreen(){
    event->keyboardMType("LWIN+L");
}

/*
 * Operation: Shutdown
 * Input: NONE
 * Return: bool, shutdown or not
 */
bool Operation::shutdown(){
    return event->exitSystem("shutdown");
}

/*
 * Operation: Shutdown Force
 * Input: NONE
 * Return: bool, shutdown force or not
 */
bool Operation::shutdownforce(){
    return event->exitSystem("shutdown_force");
}

/*
 * Operation: Reboot
 * Input: NONE
 * Return: bool, reboot or not
 */
bool Operation::reboot(){
    return event->exitSystem("reboot");
}

/*
 * Operation: Log Off
 * Input: NONE
 * Return: bool, log off or not
 */
bool Operation::logoff(){
    return event->exitSystem("logoff");
}

/*
 * Operation: Return Event Pointer
 * Input: NONE
 * Return: Event Pointer
 */
Event *Operation::events()
{
    return event;
}
