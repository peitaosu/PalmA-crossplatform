//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2018 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


#include "display.h"

Display::Display(QObject *parent) : QObject(parent){
    status_widget = new StatusWidget();
    cursor_widget = new CursorWidget();
    gesture_widget = new GestureWidget();
    palm_widget = new PalmWidget();
    dial_widget = new DialWidget();
}

/*
 * Display: Show Status Widget
 * Input: NONE
 * Return: NODE
 */
void Display::showStatus(){
    status_widget->setAvailable(true);
}

/*
 * Display: Hide Status Widget
 * Input: NONE
 * Return: NODE
 */
void Display::hideStatus(){
    status_widget->setAvailable(false);
}

/*
 * Display: Show Gesture Widget
 * Input: NONE
 * Return: NODE
 */
void Display::showGesture(){
    gesture_widget->setAvailable(true);
}

/*
 * Display: Hide Gesture Widget
 * Input: NONE
 * Return: NODE
 */
void Display::hideGesture(){
    gesture_widget->setAvailable(false);
}

/*
 * Display: Show Palm Widget
 * Input: NONE
 * Return: NODE
 */
void Display::showPalm(){
    palm_widget->setAvailable(true);
}

/*
 * Display: Hide Palm Widget
 * Input: NONE
 * Return: NODE
 */
void Display::hidePalm(){
    palm_widget->setAvailable(false);
}

/*
 * Display: Show Dial Widget
 * Input: QString dial type, double x and double y
 * Return: NODE
 */
void Display::showDial(QString dial_type, double x, double y){
    dial_widget->setDial(dial_type);
    dial_widget->setPosition(x, y);
    dial_widget->setAvailable(true);
}

/*
 * Display: Hide Dial Widget
 * Input: NONE
 * Return: NODE
 */
void Display::hideDial(){
    dial_widget->setAvailable(false);
}

/*
 * Display: Update Dial Widget
 * Input: double x, double y
 * Return: NODE
 */
void Display::updateDial(double x, double y){
    dial_widget->setTargetPosition(x, y);
}

/*
 * Display: Update Cursor Widget
 * Input: double x, double y
 * Return: NODE
 */
void Display::updateCursor(double x, double y){
    cursor_widget->updateCursor(x, y);
}

/*
 * Display: Update Status Widget
 * Input: int status code, use 3 index array to save status
 * Return: NODE
 */
void Display::updateStatus(int status_code){
    status_widget->setStatusDisplay(status_code);
}

/*
 * Display: Set Gesture Display
 * Input: QString gesture type, which same as file name in resource
 * Return: NODE
 */
void Display::setGestureDisplay(QString gesture_type){
    gesture_widget->setGestureDisplay(gesture_type);
}

/*
 * Display: Return Status Widget Pointer
 * Input: NODE
 * Return: Status Widget pointer
 */
StatusWidget * Display::widgetStatus(){
    return status_widget;
}

/*
 * Display: Return Cursor Widget Pointer
 * Input: NODE
 * Return: Cursor Widget pointer
 */
CursorWidget * Display::widgetCursor(){
    return cursor_widget;
}

/*
 * Display: Return Gesture Widget Pointer
 * Input: NODE
 * Return: Gesture Widget pointer
 */
GestureWidget * Display::widgetGesture(){
    return gesture_widget;
}

/*
 * Display: Return Dial Widget Pointer
 * Input: NODE
 * Return: Dial Widget pointer
 */
DialWidget * Display::widgetDial(){
    return dial_widget;
}

/*
 * Display: Return Palm Widget Pointer
 * Input: NODE
 * Return: Palm Widget pointer
 */
PalmWidget * Display::widgetPalm(){
    return palm_widget;
}

