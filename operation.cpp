#include "operation.h"
#include "error_code.h"
#include <QUrl>

#define SIDE_UP 1
#define SIDE_DOWN 2
#define SIDE_LEFT 3
#define SIDE_RIGHT 4

Operation::Operation(){
    //
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
    event.keyboardPress("ALT");
    while(count > 0){
        event.keyboardType("TAB");
    }
    event.keyboardRlelease("ALT");
    return 0;
}

/*
 * Operation: Move Window Side
 * Input: int side, SIDE_UP, SIDE_DOWN, SIDE_LEFT, SIDE_RIGHT defined in the file head
 */
int Operation::moveWindow(int side){
    switch(side){
        case SIDE_UP:
            event.keyboardMType("WIN+UP");
            break;
        case SIDE_DOWN:
            event.keyboardMType("WIN+DOWN");
            break;
        case SIDE_LEFT:
            event.keyboardMType("WIN+LEFT");
            break;
        case SIDE_RIGHT:
            event.keyboardMType("WIN+RIGHT");
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
    event.keyboardMType("WIN+D");
    return 0;
}

//FileManager
/*
 * Operation: Open FileManager
 */
int Operation::openFileManager(){
    event.keyboardMType("WIN+E");
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
    event.keyboardType("ENTER");
    return 0;
}

int Operation::goBack(){
    event.keyboardType("BACKSPACE");
    return 0;
}

/*
 * Operation: Refresh the Page
 */
int Operation::goRefresh(){
    event.keyboardType("F5");
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
    event.keyboardMType("CTRL+TAB");
    return 0;
}

//System
int Operation::lockscreen(){
    
}
bool Operation::shutdown(){
    
}
bool Operation::shutdownforce(){
    
}
bool Operation::reboot(){
    
}
bool Operation::logout(){
    
}


