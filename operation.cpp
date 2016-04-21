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
    //
}

void Operation::mouseSelect(double x, double y, bool press){
    event->mouseMove(x, y);
    if(press == true){
        event->mousePress(1);
    }else{
        event->mouseRelease(1);
    }
}

void Operation::mouseSelect(double x, double y){
    event->mouseMove(x, y);
}

void Operation::mouseRoll(int distance){
    event->mouseRoll(distance);
}


void Operation::openSomething(double x, double y){
    event->mouseMove(x, y);
    event->mouseDClick(1);
}

void execProgram(QString program_path){
    QProcess::startDetached(program_path ,QStringList());
}
//Window
/*
 * Operation: Swipe Window
 * Input: int count, count > 0
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

//Desktop
/*
 * Operation: Show Desktop
 */
int Operation::showDesktop(){
    event->keyboardMType("LWIN+D");
    return 0;
}

//FileManager
/*
 * Operation: Open FileManager
 */
int Operation::openFileManager(){
    event->keyboardMType("LWIN+E");
    return 0;
}

//Browser
/*
 * Operation: Open Browser, Goto Homepage Default
 * Input: QString url
 */
int Operation::openBrowser(QString url){
    QUrl url_go(url);
    QDesktopServices::openUrl(url_go);
    return 0;
}

/*
 * Operation: Goto
 */
int Operation::goTo(){
    event->keyboardType("ENTER");
    return 0;
}

int Operation::goBack(){
    event->keyboardType("BACKSPACE");
    return 0;
}

/*
 * Operation: Refresh the Page
 */
int Operation::goRefresh(){
    event->keyboardType("F5");
    return 0;
}

/*
 * Operation: Swipe Browser Tab
 * Input: int count, count > 0
 */
int Operation::swipeBrowserTab(int count){
    if(count < 1){
        return INPUT_ERROR;
    }
    event->keyboardMType("CTRL+TAB");
    return 0;
}

//System
/*
 * Operation: Lock Screen
 */
int Operation::lockscreen(){
    event->keyboardMType("LWIN+L");
    return 0;
}

/*
 * Operation: Shutdown
 */
bool Operation::shutdown(){
    return event->exitSystem("shutdown");
}

/*
 * Operation: Shutdown Force
 */
bool Operation::shutdownforce(){
    return event->exitSystem("shutdown_force");
}

/*
 * Operation: Reboot
 */
bool Operation::reboot(){
    return event->exitSystem("reboot");
}

/*
 * Operation: Log Off
 */
bool Operation::logoff(){
    return event->exitSystem("logoff");
}

Event *Operation::events()
{
    return event;
}
