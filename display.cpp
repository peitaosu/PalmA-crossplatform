#include "display.h"

Display::Display(QObject *parent) : QObject(parent)
{
    status_widget = new StatusWidget();
    cursor_widget = new CursorWidget();
    //cursor_widget->show();
    //cursor_widget->setAvailable(false);
    gesture_widget = new GestureWidget();
    //gesture_widget->show();
}
