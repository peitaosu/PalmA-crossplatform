#include <stdint.h>
#include <stdio.h>
#include <ApplicationServices/ApplicationServices.h>
#include <QFile>
#include <QJsonDocument>
#include <QCursor>
#include "event.h"
#include "error_code.h"

Event::Event(){

    //load virtual key code json file as QVariantMap
    QString vk_code_string;
    QFile vk_code_file;
    //file for macOS is under mac/
    vk_code_file.setFileName(":/mac/event_keyboard.json");
    vk_code_file.open(QIODevice::ReadOnly | QIODevice::Text);
    vk_code_string = vk_code_file.readAll();
    vk_code_file.close();
    //convert from QFile to QJsonDocument
    QJsonDocument vk_code_doc = QJsonDocument::fromJson(vk_code_string.toUtf8());
    //convert yo VariantMap
    virtual_key_code = vk_code_doc.toVariant().toMap();
}

Event::~Event(){
    
}

int Event::mouseMove(double x, double y){
    if(x < 0 || x > 1 || y < 0 || y > 1){
        return INPUT_ERROR;;
    }
    //the cursor mouse position is following the display resolution
    CGPoint point;
    point.x = x * QApplication::desktop()->width();
    point.y = y * QApplication::desktop()->height();

    CGEventRef move;
    move = CGEventCreateMouseEvent(NULL, kCGEventMouseMoved, point, kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, move);
    return 0;
}

int Event::mousePress(int left_or_right)
{
    if(left_or_right == 1){
        //left button down
        CGEventRef press = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, QCursor::pos().toCGPoint(), kCGMouseButtonLeft);
        CGEventPost(kCGHIDEventTap, press);
        return 0;
    }else if(left_or_right == 2){
        //right button down
        CGEventRef press = CGEventCreateMouseEvent(NULL, kCGEventRightMouseDown, QCursor::pos().toCGPoint(), kCGMouseButtonRight);
        CGEventPost(kCGHIDEventTap, press);
        return 0;
    }else {
        return INPUT_ERROR;
    }
}

int Event::mouseRelease(int left_or_right)
{
    if(left_or_right == 1){
        //left button up
        CGEventRef release = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, QCursor::pos().toCGPoint(), kCGMouseButtonLeft);
        CGEventPost(kCGHIDEventTap, release);
        return 0;
    }else if(left_or_right == 2){
        //right button up
        CGEventRef release = CGEventCreateMouseEvent(NULL, kCGEventRightMouseUp, QCursor::pos().toCGPoint(), kCGMouseButtonRight);
        CGEventPost(kCGHIDEventTap, release);
        return 0;
    }else {
        return INPUT_ERROR;
    }
}

int Event::mouseClick(int left_or_right)
{
    if(left_or_right == 1){
        //left button click
        CGEventRef press = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, QCursor::pos().toCGPoint(), kCGMouseButtonLeft);
        CGEventPost(kCGHIDEventTap, press);
        CGEventRef release = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, QCursor::pos().toCGPoint(), kCGMouseButtonLeft);
        CGEventPost(kCGHIDEventTap, release);
        return 0;
    }else if(left_or_right == 2){
        //right button click
        CGEventRef press = CGEventCreateMouseEvent(NULL, kCGEventRightMouseDown, QCursor::pos().toCGPoint(), kCGMouseButtonRight);
        CGEventPost(kCGHIDEventTap, press);
        CGEventRef release = CGEventCreateMouseEvent(NULL, kCGEventRightMouseUp, QCursor::pos().toCGPoint(), kCGMouseButtonRight);
        CGEventPost(kCGHIDEventTap, release);
        return 0;
    }else {
        return INPUT_ERROR;
    }
}

int Event::mouseDClick(int left_or_right)
{
    if(left_or_right == 1){
        //left button click twice
        mouseClick(1);
        mouseClick(1);
        return 0;
    }else if(left_or_right == 2){
        //rigth button click twice
        mouseClick(2);
        mouseClick(2);
        return 0;
    }else {
        return INPUT_ERROR;
    }
}

void Event::mouseRoll(int distance)
{
    //mouse roll, negative will roll reverse direction
    CGEventRef scroll = CGEventCreateScrollWheelEvent(NULL, kCGScrollEventUnitPixel, 1, distance);
    CGEventPost(kCGHIDEventTap, scroll);
}