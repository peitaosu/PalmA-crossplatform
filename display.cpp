#include "display.h"

Display::Display(QObject *parent) : QObject(parent)
{
    //status_widget = new StatusWidget();
    //cursor_widget = new CursorWidget();
    //cursor_widget->show();
    //cursor_widget->setAvailable(false);
    //gesture_widget = new GestureWidget();
    //gesture_widget->show();
    dial_widget = new DialWidget();
    dial_widget->setVisible(true);
    dial_widget->setDial("desktop");
    dial_widget->setPosition(0.5, 0.5);
    dial_widget->setTargetPosition(0.6, 0.6);

}
