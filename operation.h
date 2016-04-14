#ifndef OPERATION_H
#define OPERATION_H
#include "event_win.h"
#include "event_unix.h"

namespace System {
    class Operation;
}

class Operation
{
    public:
        Operation();

        //Window
        int swipeWindow(int count = 1);
        int moveWindow(int side);
        
        //Desktop
        int showDesktop();

        //FileManager
        int openFileManager();
        
        //Browser
        int openBrowser(QString url = "about:home");
        int goTo();
        int goBack();
        int goRefresh();
        int swipeBrowserTab(int count = 1);        

        //System
        int lockscreen();
        bool shutdown();
        bool shutdownforce();
        bool reboot();
        bool logout();

    private:
        Event event;

};

#endif // OPERATION_H
