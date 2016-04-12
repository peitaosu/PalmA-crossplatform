#include "event_win.h"
#include <Windows.h>
#include <QFile>
#include <QJsonDocument>

#pragma comment (lib,"Advapi32.lib")
#pragma comment (lib,"User32.lib")


Event::Event(){

    //load virtual key code json file as QVariantMap
    QString vk_code_string;
    QFile vk_code_file;
    vk_code_file.setFileName(":/event_keyboard_win.json");
    vk_code_file.open(QIODevice::ReadOnly | QIODevice::Text);
    vk_code_string = vk_code_file.readAll();
    vk_code_file.close();
    QJsonDocument vk_code_doc = QJsonDocument::fromJson(vk_code_string.toUtf8());
    virtual_key_code = vk_code_doc.toVariant().toMap();

}

/*
 * Event: Mouse Move
 * Input: double x [0.0, 1.0], double y [0.0, 1.0]
 * Input Error: return -1;
 */
int Event::mouseMove(double x, double y){
    if(x < 0 || x > 1 || y < 0 || y > 1){
        return -1;
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
 * Input Error: return -1
 */
int Event::mousePress(int left_or_right)
{
    if(left_or_right == 1){
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        return 0;
    }else if(left_or_right == 2){
        mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
        return 0;
    }else {
        return -1;
    }
}

/*
 * Event: Mouse Release
 * Input: int left = 1, right = 2
 * Input Error: return -1
 */
int Event::mouseRelease(int left_or_right)
{
    if(left_or_right == 1){
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        return 0;
    }else if(left_or_right == 2){
        mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
        return 0;
    }else {
        return -1;
    }
}

/*
 * Event: Mouse Click
 * Input: int left = 1, right = 2
 * Input Error: return -1
 */
int Event::mouseClick(int left_or_right)
{
    if(left_or_right == 1){
        mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        return 0;
    }else if(left_or_right == 2){
        mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
        return 0;
    }else {
        return -1;
    }
}

/*
 * Event: Mouse Double Click
 * Input: int left = 1, right = 2
 * Input Error: return -1
 */
int Event::mouseDClick(int left_or_right)
{
    if(left_or_right == 1){
        mouseClick(1);
        mouseClick(1);
        return 0;
    }else if(left_or_right == 2){
        mouseClick(2);
        mouseClick(2);
        return 0;
    }else {
        return -1;
    }
}

/*
 * Event: Mouse Wheel
 * Input: int distance, default 5
 */
void Event::mouseRoll(int distance)
{
    mouse_event(MOUSEEVENTF_WHEEL, 0, 0, distance, 0);

}

/*
 * Event: Key Press
 * Input: QString key
 */
int Event::keyboardPress(QString key)
{
    if(virtual_key_code[key].isValid()){
        keybd_event(virtual_key_code[key].toInt(), 0, 0, 0);
        return 0;
    }else{
        //TODO: LOG ERROR
        return -1;
    }
}

/*
 * Event: Key Release
 * Input: QString key
 */
int Event::keyboardRelease(QString key)
{
    if(virtual_key_code[key].isValid()){
        keybd_event(virtual_key_code[key].toInt(), 0, KEYEVENTF_KEYUP, 0);
        return 0;
    }else{
        //TODO: LOG ERROR
        return -1;
    }
}

/*
 * Event: Key Type
 * Input: QString key
 */
int Event::keyboardType(QString key)
{
    if(virtual_key_code[key].isValid()){
        keybd_event(virtual_key_code[key].toInt(), 0, 0, 0);
        keybd_event(virtual_key_code[key].toInt(), 0, KEYEVENTF_KEYUP, 0);
        return 0;
    }else{
        //TODO: LOG ERROR
        return -1;
    }
}

/*
 * Event: Key Multi Type
 * Input: QString multi_key with "+"
 */
int Event::keyboardMType(QString multi_key)
{
    QStringList multi_key_list;
    multi_key_list = multi_key.split('+');
    for(int i = 0; i < multi_key_list.size(); i++){
        if(!virtual_key_code[multi_key_list[i]].isValid()){
            //TODO: LOG ERROR
            return -1;
        }
    }
    for(int i = 0; i < multi_key_list.size(); i++){
        keyboardPress(multi_key_list[i]);
    }
    for(int i = 0; i < multi_key_list.size(); i++){
        keyboardRelease(multi_key_list[i]);
    }
    return 0;
}

