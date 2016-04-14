#include "event_unix.h"
#include <QFile>
#include <QJsonDocument>
#include <QApplication>
#include <QDesktopWidget>
#include "error_code.h"
Event::Event(){

    //load virtual key code json file as QVariantMap
    QString vk_code_string;
    QFile vk_code_file;
    vk_code_file.setFileName(":/event_keyboard_unix.json");
    vk_code_file.open(QIODevice::ReadOnly | QIODevice::Text);
    vk_code_string = vk_code_file.readAll();
    vk_code_file.close();
    QJsonDocument vk_code_doc = QJsonDocument::fromJson(vk_code_string.toUtf8());
    virtual_key_code = vk_code_doc.toVariant().toMap();

    //x11 display
    display = XOpenDisplay (NULL);

}

Event::~Event(){
    XCloseDisplay (display);
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
    //in x11 display
    //the cursor mouse position is following the display resolution
    int point_x = x * QApplication::desktop()->width();
    int point_y = y * QApplication::desktop()->height();
    XTestFakeMotionEvent (display, -1, point_x, point_y, CurrentTime);
    XSync(display, 0);
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
        XTestFakeButtonEvent (display, 1, True,  CurrentTime);
        return 0;
    }else if(left_or_right == 2){
        XTestFakeButtonEvent (display, 3, True,  CurrentTime);
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
        XTestFakeButtonEvent (display, 1, False,  CurrentTime);
        return 0;
    }else if(left_or_right == 2){
        XTestFakeButtonEvent (display, 3, False,  CurrentTime);
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
        XTestFakeButtonEvent (display, 1, True,  CurrentTime);
        XTestFakeButtonEvent (display, 1, False,  CurrentTime);
        return 0;
    }else if(left_or_right == 2){
        XTestFakeButtonEvent (display, 3, True,  CurrentTime);
        XTestFakeButtonEvent (display, 3, False,  CurrentTime);
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
        mouseClick(1);
        mouseClick(1);
        return 0;
    }else if(left_or_right == 2){
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
    int timer = distance;
    while(timer > 0){
        XTestFakeButtonEvent (display, 5, True,  CurrentTime);
        timer --;
    }
    XTestFakeButtonEvent (display, 5, False,  CurrentTime);
}

/*
 * Event: Key Press
 * Input: QString key
 */
int Event::keyboardPress(QString key)
{
    if(virtual_key_code[key].isValid()){
        XTestFakeKeyEvent (display, virtual_key_code[key].toInt(), True, 0);
        return 0;
    }else{
        //TODO: LOG ERROR
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
        XTestFakeKeyEvent (display, virtual_key_code[key].toInt(), False, 0);
        return 0;
    }else{
        //TODO: LOG ERROR
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
        XTestFakeKeyEvent (display, virtual_key_code[key].toInt(), True, 0);
        XTestFakeKeyEvent (display, virtual_key_code[key].toInt(), False, 0);
        return 0;
    }else{
        //TODO: LOG ERROR
        return INPUT_ERROR;
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
            return INPUT_ERROR;
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
