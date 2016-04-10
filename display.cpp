#include "display.h"

Display::Display(QObject *parent) : QObject(parent)
{
    status_widget = new StatusWidget();
    status_widget->show();
}
