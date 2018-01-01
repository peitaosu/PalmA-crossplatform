//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2018 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


#include "event.h"
#include <Windows.h>
#include <QFile>
#include <QJsonDocument>
#include "error_code.h"

#pragma comment (lib,"Advapi32.lib")
#pragma comment (lib,"User32.lib")


Event::Event(){

    //load virtual key code json file as QVariantMap
    QString vk_code_string;
    QFile vk_code_file;
    //file for Windows location is under win/
    vk_code_file.setFileName(":/win/event_keyboard.json");
    vk_code_file.open(QIODevice::ReadOnly | QIODevice::Text);
    vk_code_string = vk_code_file.readAll();
    vk_code_file.close();
    //convert from QFile to QJsonDocument
    QJsonDocument vk_code_doc = QJsonDocument::fromJson(vk_code_string.toUtf8());
    //convert yo VariantMap
    virtual_key_code = vk_code_doc.toVariant().toMap();

}

/*
 * Event: Mouse Move
 * Input: double x [0.0, 1.0], double y [0.0, 1.0]
 * Input Error: return INPUT_ERROR;
 */
int Event::mouseMove(double x, double y){
    if(x < 0 || x > 1 || y < 0 || y > 1){
        return INPUT_ERROR;
    }
    //in windows display
    //the cursor mouse position is 0~65535 in x-axis and 0~65535 in y-axis
    int point_x = x * 65535;
    int point_y = y * 65535;
    mouse_event(MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE, point_x, point_y, 0, 0);
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
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        return 0;
    }else if(left_or_right == 2){
        //right button down
        mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
        return 0;
    }else {
        return INPUT_ERROR;
    }
}

/*
 * Event: Mouse Release
 * Input: int left = 1, right = 2
 * Input Error: return INPUT_ERROR
 */
int Event::mouseRelease(int left_or_right)
{
    if(left_or_right == 1){
        //left button up
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        return 0;
    }else if(left_or_right == 2){
        //right button up
        mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
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
        mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        return 0;
    }else if(left_or_right == 2){
        //right button click
        mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
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
    mouse_event(MOUSEEVENTF_WHEEL, 0, 0, distance, 0);
}

/*
 * Event: Key Press
 * Input: QString key
 */
int Event::keyboardPress(QString key)
{
    if(virtual_key_code[key].isValid()){
        //convert virtual_key_code to integer, as input of keybd_event, 0 means keydown
        keybd_event(virtual_key_code[key].toInt(), 0, 0, 0);
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
        //convert virtual_key_code to integer, as input of keybd_event, KEYEVENTF_KEYUP means keyup
        keybd_event(virtual_key_code[key].toInt(), 0, KEYEVENTF_KEYUP, 0);
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
        keybd_event(virtual_key_code[key].toInt(), 0, 0, 0);
        keybd_event(virtual_key_code[key].toInt(), 0, KEYEVENTF_KEYUP, 0);
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
 */
bool Event::exitSystem(QString exit_event){
    //get ready for system shutdown/reoot/logoff
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
      return( FALSE );

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
        &tkp.Privileges[0].Luid);

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
        (PTOKEN_PRIVILEGES)NULL, 0);

    if (GetLastError() != ERROR_SUCCESS)
      return FALSE;

    //shutdown, will run the shutdown process
    if(exit_event == "shutdown"){
        if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_POWEROFF,
        SHTDN_REASON_MAJOR_OPERATINGSYSTEM | 
        SHTDN_REASON_MINOR_UPGRADE | 
        SHTDN_REASON_FLAG_PLANNED))
            return FALSE;
    }
    
    //shutdown force, don`t run the shutdown process, file unsaved will lot
    if(exit_event == "shutdown_force"){
        if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 
        SHTDN_REASON_MAJOR_OPERATINGSYSTEM | 
        SHTDN_REASON_MINOR_UPGRADE | 
        SHTDN_REASON_FLAG_PLANNED))
            return FALSE;
    }
    
    //reboot
    if(exit_event == "reboot"){
        if (!ExitWindowsEx(EWX_REBOOT, 
        SHTDN_REASON_MAJOR_OPERATINGSYSTEM | 
        SHTDN_REASON_MINOR_UPGRADE | 
        SHTDN_REASON_FLAG_PLANNED))
            return FALSE;
    }
    
    //logoff
    if(exit_event == "logoff"){
        if (!ExitWindowsEx(EWX_LOGOFF, 
        SHTDN_REASON_MAJOR_OPERATINGSYSTEM | 
        SHTDN_REASON_MINOR_UPGRADE | 
        SHTDN_REASON_FLAG_PLANNED))
            return FALSE;
    }
    return TRUE;
}

