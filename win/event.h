#ifndef EVENT_H
#define EVENT_H

#include <QVariantMap>

namespace System {
    class Event;
}

class Event
{
    public:
        Event();

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

};

#endif // EVENT_H
