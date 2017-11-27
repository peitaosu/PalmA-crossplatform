//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2017 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


#include "event.h"
#include "error_code.h"
#include <stdint.h>
#include <stdio.h>
#include <ApplicationServices/ApplicationServices.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QFile>
#include <QJsonDocument>
#include <QCursor>

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

/*
 * Event: Mouse Move
 * Input: double x [0.0, 1.0], double y [0.0, 1.0]
 * Input Error: return INPUT_ERROR;
 */
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

/*
 * Event: Mouse Press
 * Input: int left = 1, right = 2
 * Input Error: return INPUT_ERROR
 */
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

/*
 * Event: Mouse Release
 * Input: int left = 1, right = 2
 * Input Error: return INPUT_ERROR;
 */
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

/*
 * Event: Mouse Click
 * Input: int left = 1, right = 2
 * Input Error: return INPUT_ERROR
 */
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

/*
 * Event: Mouse Double Click
 * Input: int left = 1, right = 2
 * Input Error: return INPUT_ERROR
 */
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

/*
 * Event: Mouse Wheel
 * Input: int distance, default 5
 */
void Event::mouseRoll(int distance)
{
    //mouse roll, negative will roll reverse direction
    CGEventRef scroll = CGEventCreateScrollWheelEvent(NULL, kCGScrollEventUnitPixel, 1, distance);
    CGEventPost(kCGHIDEventTap, scroll);
}

/*
 * Event: Key Press
 * Input: QString key
 */
int Event::keyboardPress(QString key)
{
    if(virtual_key_code[key].isValid()){
        //convert virtual_key_code to integer, as input of keybd_event, True means keydown
        CGEventRef press = CGEventCreateKeyboardEvent(NULL, virtual_key_code[key].toInt(), true);
        CGEventPost(kCGHIDEventTap, press);
        return 0;
    }else{
        return INPUT_ERROR;
    }
}

/*
 * Event: Key Release
 * Input: QString key
 */
int Event::keyboardRelease(QString key)
{
    if(virtual_key_code[key].isValid()){
        //convert virtual_key_code to integer, as input of keybd_event, False means keyup
        CGEventRef release = CGEventCreateKeyboardEvent(NULL, virtual_key_code[key].toInt(), false);
        CGEventPost(kCGHIDEventTap, release);
        return 0;
    }else{
        return INPUT_ERROR;
    }
}

/*
 * Event: Key Type
 * Input: QString key
 */
int Event::keyboardType(QString key)
{
    if(virtual_key_code[key].isValid()){
        //key type
        CGEventRef press = CGEventCreateKeyboardEvent(NULL, virtual_key_code[key].toInt(), true);
        CGEventPost(kCGHIDEventTap, press);
        CGEventRef release = CGEventCreateKeyboardEvent(NULL, virtual_key_code[key].toInt(), false);
        CGEventPost(kCGHIDEventTap, release);
        return 0;
    }else{
        return INPUT_ERROR;
    }
}

/*
 * Event: Key Multi Type
 * Input: QString multi_key with "+"
 */
int Event::keyboardMType(QString multi_key)
{
    //split the multi_key with "+"
    QStringList multi_key_list;
    multi_key_list = multi_key.split('+');
    for(int i = 0; i < multi_key_list.size(); i++){
        if(!virtual_key_code[multi_key_list[i]].isValid()){
            return INPUT_ERROR;
        }
    }
    //keys in list press
    for(int i = 0; i < multi_key_list.size(); i++){
        keyboardPress(multi_key_list[i]);
    }
    //keys in list release
    for(int i = 0; i < multi_key_list.size(); i++){
        keyboardRelease(multi_key_list[i]);
    }
    return 0;
}

/*
 * Event: Exit System
 * Input: QString exit_event
 * Linux not support logout via terminal.
 */
bool Event::exitSystem(QString exit_event){

    //will execute system command, need root permission
    
    //shutdown
    if(exit_event == "shutdown"){
        if(!process.startDetached("shutdown -h now")){
            return FALSE;
        }
    }

    //shutdown force, in fact same as shutdown 
    if(exit_event == "shutdown_force"){
        if(!process.startDetached("shutdown -h now")){
            return FALSE;
        }
    }

    //reboot
    if(exit_event == "reboot"){
        if(!process.startDetached("shutdown -r now")){
            return FALSE;
        }
    }

    //logoff
    if(exit_event == "logoff"){
        if(!process.startDetached("exit")){
            return FALSE;
        }
    }
    return TRUE;
}
