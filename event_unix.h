#ifndef EVENT_H
#define EVENT_H

#include <QVariantMap>
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

    private:
        QVariantMap virtual_key_code;
        Display *display;
        XEvent event;

};

#endif // EVENT_H
