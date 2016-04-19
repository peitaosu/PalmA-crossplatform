#include "display.h"

Display::Display(QObject *parent) : QObject(parent)
{
    status_widget = new StatusWidget();
    status_widget->setAvailable(false);
    cursor_widget = new CursorWidget();
    cursor_widget->setAvailable(true);
    gesture_widget = new GestureWidget();
    //gesture_widget->setAvailable(true);
    palm_widget = new PalmWidget();
    palm_widget->setAvailable(false);
    dial_widget = new DialWidget();
    //cursor_widget->show();
    //cursor_widget->setAvailable(false);
    //gesture_widget->show();
    //dial_widget->setVisible(false);
    //dial_widget->setDial("desktop");
    //dial_widget->setPosition(0.5, 0.5);
    //dial_widget->setTargetPosition(0.6, 0.6);


}

void Display::showStatus()
{
    status_widget->setAvailable(true);
}

void Display::hideStatus()
{
    status_widget->setAvailable(false);
}

void Display::showGesture()
{
    gesture_widget->setAvailable(true);
}

void Display::hideGesture()
{
    gesture_widget->setAvailable(false);
}

void Display::showPalm()
{
    palm_widget->setAvailable(true);
}

void Display::hidePalm()
{
    palm_widget->setAvailable(false);
}

void Display::showDial(QString dial_type, double x, double y){
    dial_widget->setDial(dial_type);
    dial_widget->setPosition(x, y);
    dial_widget->setAvailable(true);
}

void Display::hideDial(){
    dial_widget->setAvailable(false);
}

void Display::updateDial(double x, double y){
    dial_widget->setTargetPosition(x, y);
}

void Display::updateCursor(double x, double y)
{
    cursor_widget->updateCursor(x, y);

}

void Display::updateStatus(int status_code)
{
    status_widget->setStatusDisplay(status_code);
}

void Display::setGestureDisplay(QString gesture_type)
{
    gesture_widget->setGestureDisplay(gesture_type);
}
