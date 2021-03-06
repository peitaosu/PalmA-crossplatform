//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2019 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


#ifndef PALMA_EVENT_H
#define PALMA_EVENT_H

#include <QVariantMap>
#include <QProcess>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>

namespace System {
    class Event;
}

class Event
{
    public:
        Event();
        ~Event();

        //mouse events
        int mouseMove(double x, double y);
        int mousePress(int left_or_right = 1);
        int mouseRelease(int left_or_right = 1);
        int mouseClick(int left_or_right = 1);
        int mouseDClick(int left_or_right = 1);
        void mouseRoll(int distance = 5);

        //keyboard operation
        int keyboardPress(QString key);
        int keyboardRelease(QString key);
        int keyboardType(QString key);
        int keyboardMType(QString multi_key);

        //system
        bool exitSystem(QString exit_event);

    private:
        //map for key : virtual key code
        QVariantMap virtual_key_code;
        //process for command execution
        QProcess process;
        //x11 display
        Display *display;
        //XTest event
        XEvent event;

};

#endif // PALMA_EVENT_H
