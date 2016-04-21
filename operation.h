#ifndef OPERATION_H
#define OPERATION_H
#include "event.h"

#include <QObject>
namespace System {
    class Operation;
}

class Operation : public QObject
{
        Q_OBJECT
    public:
        explicit Operation(QObject *parent = 0);
    
    public slots:
        //mouse
        void mouseSelect(double x, double y, bool press);
        void mouseSelect(double x, double y);
        void mouseRoll(int distance = 5);
        void openSomething(double x, double y);
        
        //exec program
        void execProgram(QString program_path);
        
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
        bool logoff();

        Event *events();

    private:
        Event *event;

};

#endif // OPERATION_H
