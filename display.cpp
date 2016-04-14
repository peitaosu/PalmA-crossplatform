#include "display.h"

Display::Display(QObject *parent) : QObject(parent)
{
    //status_widget = new StatusWidget();
    //status_widget->show();
    cursor_widget = new CursorWidget();
    cursor_widget->show();
    cursor_widget->setAvailable(false);
}
