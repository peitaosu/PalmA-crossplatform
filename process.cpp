#include "process.h"
#include "motion_code.h"
#include "foreground.h"
#include <QJsonDocument>
#include <QDate>
Process::Process(QObject *parent) : QObject(parent)
{
    loop_timer = new QTimer(this);

    QString config_sring;
    QFile config_file;
    config_file.setFileName(":/config.json");
    config_file.open(QIODevice::ReadOnly | QIODevice::Text);
    config_sring = config_file.readAll();
    config_file.close();
    QJsonDocument config_json = QJsonDocument::fromJson(config_sring.toUtf8());
    config = config_json.toVariant().toMap();
    config_gesture = config["gesture"].toMap();
    config_display = config["display"].toMap();
    config_operation = config["operation"].toMap();
    config = config["config"].toMap();
    

    connect(this, SIGNAL(setGesture(QString)), this, SLOT(showGesture(QString)));
    connect(this, SIGNAL(showDial(bool)), display.widgetDial(), SLOT(setAvailable(bool)));
    connect(this, SIGNAL(showDial(double, double)), display.widgetDial(), SLOT(setPosition(double, double)));
    connect(this, SIGNAL(updateDial(double, double)), display.widgetDial(), SLOT(setTargetPosition(double, double)));
    connect(display.widgetDial(), SIGNAL(choose_up()), this, SLOT(dial_up()));
    connect(display.widgetDial(), SIGNAL(choose_down()), this, SLOT(dial_down()));
    connect(display.widgetDial(), SIGNAL(choose_left()), this, SLOT(dial_left()));
    connect(display.widgetDial(), SIGNAL(choose_right()), this, SLOT(dial_right()));
    

}

void Process::init(int argc, char* argv[])
{
    
    if(config_display["palm"].toString() == "true"){
        display.showPalm();
    }else if(config_display["palm"].toString() == "false"){
        display.hidePalm();
    }
    if(config_display["gesture"].toString() == "true"){
        display.showGesture();
    }else if(config_display["gesture"].toString() == "false"){
        display.hideGesture();
    }
    if(config_display["status"].toString() == "true"){
        display.showStatus();
    }else if(config_display["status"].toString() == "false"){
        display.hideStatus();
    }
    for(int arg = 1; arg < argc; arg ++){
        if(argv[arg] == "-d"){
            display.showStatus();
        }else if(argv[arg] == "-s"){
            display.hideGesture();
        }else if(argv[arg] == "-h"){
            display.showPalm();
        }
    }
    
    if(config["controller"].toString() == "LEAPMOTION"){
        motion.setController(LEAP_MOTION);
    }
    
    if(config["handedness"].toString() == "left"){
        motion.setHandedness(true);
    }else if(config["handedness"].toString() == "right"){
        motion.setHandedness(false);
    }
       
    QDate date;
    QString current_date = date.currentDate().toString();
    QString log_file_name = current_date + ".log";
 
    if(config["log"].isNull()){
        logger.setLogDir("\\log");
        logger.setLogFile(log_file_name);
    }else{
        logger.setLogDir(config["log"].toString());
        logger.setLogFile(log_file_name);
    }
    
    /*for test
    display.showStatus();
    display.showGesture();
    display.showPalm();
    */
    //load gesture:operation mapping

}

void Process::start()
{
    connect(loop_timer,SIGNAL(timeout()),this,SLOT(run()));
    loop_timer->start(20);
}

void Process::run()
{
    
    motion.update();
    //motion.setHandedness();
    if(motion.getHandCount() == 0){
        emit setGesture("off");
    }else{
        display.updateCursor(motion.getNormalizedX(), motion.getNormalizedY());
        display.updateStatus(motion.getControllerStatus()*4 + motion.getServiceStatus()*2 + motion.getProcessStatus());

        QVariantMap config_runtime = config_gesture[getForegroundWindow()].toMap();
        display.widgetDial()->setDial(getForegroundWindow());
        if(motion.getHandCount() == 2){
            //isAllGrab?
            if(config_runtime["grab_2hands"].toString() == "true"){
                int grab_status_with_hands = motion.getGestureEvent(GRAB, 2);
                switch (grab_status_with_hands){
                    case NONE:
                        emit grab(NONE);
                        break;
                    case START:
                        emit grab(START);
                        emit setGesture("grab");
                        break;
                    case KEEP:
                        emit grab(KEEP);
                        emit setGesture("grab");
                        break;
                    case STOP:
                        emit grab(STOP);
                        emit setGesture("hand");
                        break;
                    default:
                        break;
                }
            }
            //isOneGrab?
            if(config_runtime["grab"].toString() == "true"){
                int grab_status = motion.getGestureEvent(GRAB, 1);
                switch (grab_status){
                    case NONE:
                        emit grab(NONE);
                        break;
                    case START:
                        emit grab(motion.getNormalizedX(), motion.getNormalizedY(), 1);
                        emit setGesture("grab");
                        break;
                    case KEEP:
                        emit grab(motion.getNormalizedX(), motion.getNormalizedY());
                        emit setGesture("grab");
                        break;
                    case STOP:
                        emit grab(motion.getNormalizedX(), motion.getNormalizedY(), 0);
                        emit setGesture("hand");
                        break;
                    default:
                        break;
                }
            }
            //isAllPinch?
            if(config_runtime["pinch_2hands"].toString() == "true"){
                int pinch_status_with_hands = motion.getGestureEvent(PINCH, 2);
                switch (pinch_status_with_hands){
                    case NONE:
                        emit pinch(NONE);
                        break;
                    case START:
                        emit pinch(START);
                        emit setGesture("pinch");
                        break;
                    case KEEP:
                        emit pinch(KEEP);
                        emit setGesture("pinch");
                        break;
                    case STOP:
                        emit pinch(STOP);
                        emit setGesture("pinche");
                        break;
                    default:
                        break;
                }
            }
            //isOnePinch?
            if(config_runtime["pinch"] == "true"){
                int pinch_status = motion.getGestureEvent(PINCH, 1);
                switch (pinch_status){
                    case NONE:
                        emit pinch(NONE);
                        break;
                    case START:
                        emit pinch(motion.getNormalizedX(), motion.getNormalizedY(), START);
                        emit setGesture("pinch");
                        break;
                    case KEEP:
                        emit pinch(motion.getNormalizedX(), motion.getNormalizedY());
                        emit setGesture("pinch");
                        break;
                    case STOP:
                        emit pinch(motion.getNormalizedX(), motion.getNormalizedY(), STOP);
                        emit setGesture("pinche");
                        break;
                    default:
                        break;
                }
            }
        }else if(motion.getHandCount() == 1){
            //isGrab?
            if(config_runtime["grab"].toString() == "true"){
                int grab_status = motion.getGestureEvent(GRAB, 1);
                switch (grab_status){
                    case NONE:
                        emit grab(NONE);
                        break;
                    case START:
                        emit grab(motion.getNormalizedX(), motion.getNormalizedY(), START);
                        emit setGesture("grab");
                        break;
                    case KEEP:
                        emit grab(motion.getNormalizedX(), motion.getNormalizedY());
                        emit setGesture("grab");
                        break;
                    case STOP:
                        emit grab(motion.getNormalizedX(), motion.getNormalizedY(), STOP);
                        emit setGesture("hand");
                        break;
                    default:
                        break;
                }
            }
            //isPinch?
            if(config_runtime["pinch"].toString() == "true"){
                int pinch_status = motion.getGestureEvent(PINCH, 1);
                switch (pinch_status){
                    case NONE:
                        emit pinch(NONE);
                        break;
                    case START:
                        emit pinch(motion.getNormalizedX(), motion.getNormalizedY(), START);
                        emit setGesture("pinch");
                        emit showDial(true);
                        emit showDial(motion.getNormalizedX(), motion.getNormalizedY());
                        break;
                    case KEEP:
                        emit pinch(motion.getNormalizedX(), motion.getNormalizedY());
                        emit setGesture("pinch");
                        emit updateDial(motion.getNormalizedX(), motion.getNormalizedY());
                        break;
                    case STOP:
                        emit pinch(motion.getNormalizedX(), motion.getNormalizedY(), STOP);
                        //emit setGesture("pinche");
                        emit setGesture("hand");
                        emit showDial(false);
                        break;
                    default:
                        break;
                }
            }
            //isCircle?
            if(config_runtime["circle"].toString() == "true"){
                int circle_status = motion.getGestureEvent(CIRCLE, 1);
                switch (circle_status){
                    case NONE:
                        emit circle(NONE);
                        break;
                    case START:
                        emit circle(START);
                        emit setGesture("circle");
                        break;
                    case KEEP:
                        emit circle(KEEP);
                        emit setGesture("circle");
                        break;
                    case STOP:
                        emit circle(STOP);
                        emit setGesture("hand");
                        break;
                    default:
                        break;
                }
            }
            //isCircle-Anti?
            if(config_runtime["circle_anti"].toString() == "true"){
                int circle_anti_status = motion.getGestureEvent(CIRANTI, 1);
                switch (circle_anti_status){
                    case NONE:
                        emit circle_anti(NONE);
                        break;
                    case START:
                        emit circle_anti(START);
                        emit setGesture("circle_anti");
                        break;
                    case KEEP:
                        emit circle_anti(KEEP);
                        emit setGesture("circle_anti");
                        break;
                    case STOP:
                        emit circle_anti(STOP);
                        emit setGesture("hand");
                        break;
                    default:
                        break;
                }
            }
            //isSwipe?
            if(config_runtime["swipe"].toString() == "true"){
                int swipe_status = motion.getGestureEvent(SWIPE, 1);
                switch (swipe_status){
                    case NONE:
                        emit swipe(NONE);
                        break;
                    case START:
                        emit swipe(START);
                        emit setGesture("swipe");
                        break;
                    case KEEP:
                        emit swipe(KEEP);
                        emit setGesture("swipe");
                        break;
                    case STOP:
                        emit swipe(STOP);
                        emit setGesture("hand");
                        break;
                    default:
                        break;
                }
            }
            //isScreenTap?
            if(config_runtime["screen_tap"].toString() == "true"){
                int screen_tap_status = motion.getGestureEvent(SCREENTAP, 1);
                switch (screen_tap_status){
                    case NONE:
                        emit screen_tap(NONE);
                        break;
                    case START:
                        emit screen_tap(motion.getNormalizedX(), motion.getNormalizedY(), START);
                        break;
                    case KEEP:
                        emit screen_tap(KEEP);
                        break;
                    case STOP:
                        emit screen_tap(STOP);
                        break;
                    default:
                        break;
                }
            }
            //isKeyTap?
            if(config_runtime["key_tap"].toString() == "true"){
                int key_tap_status = motion.getGestureEvent(KEYTAP, 1);
                switch (key_tap_status){
                    case NONE:
                        emit key_tap(NONE);
                        break;
                    case START:
                        emit key_tap(motion.getNormalizedX(), motion.getNormalizedY(), START);
                        break;
                    case KEEP:
                        emit key_tap(KEEP);
                        break;
                    case STOP:
                        emit key_tap(STOP);
                        break;
                    default:
                        break;
                }
            }
            //isHold?
            //emit hold();
        }
    }
}
void Process::stop()
{
    disconnect(loop_timer,SIGNAL(timeout()),this,SLOT(run()));
}

void Process::restart()
{
    stop();
    start();
}

void Process::showGesture(QString gesture_type)
{
    display.showGesture();
    display.setGestureDisplay(gesture_type);
}

void Process::changedToDesktop()
{
    disconnectAll();
    QVariantMap config_operation_desktop = config_operation["desktop"].toMap();
    QVariantMap config_operation_desktop_dial = config_operation_desktop["dial"].toMap();
    if(config_operation_desktop_dial["up"].toString() == "opensometing"){
        connect(display.widgetDial(), SIGNAL(choose_up(double, double)), operation, SLOT(openSometing(double, double)));
    }else{
        connect(display.widgetDial(), SIGNAL(choose_up(QString)), operation, SLOT(execProgram(QString)));
    }
    if(config_operation_desktop_dial["down"].toString() == "setting"){
        connect(display.widgetDial(), SIGNAL(choose_down()), operation, SLOT(lockscreen()));
    }else{
        connect(display.widgetDial(), SIGNAL(choose_down(QString)), operation, SLOT(execProgram(QString)));
    }
    if(config_operation_desktop_dial["left"].toString() == "browser"){
        connect(display.widgetDial(), SIGNAL(choose_left()), operation, SLOT(openBrowser()));
    }else{
        connect(display.widgetDial(), SIGNAL(choose_left(QString)), operation, SLOT(execProgram(QString)));
    }
    if(config_operation_desktop_dial["right"].toString() == "explorer"){
        connect(display.widgetDial(), SIGNAL(choose_right()), operation, SLOT(openFileManager()));
    }else{
        connect(display.widgetDial(), SIGNAL(choose_right(QString)), operation, SLOT(execProgram(QString)));
    }
    /*
    connect(this, SIGNAL(swipe(int)), operation, SLOT());
    connect(this, SIGNAL(circle(int)), operation, SLOT());
    connect(this, SIGNAL(circle_anti(int)), operation(), SLOT());
    connect(this, SIGNAL(screen_tap(double,double,int)), operation(), SLOT());
    connect(this, SIGNAL(screen_tap(int)), operation(), SLOT());
    connect(this, SIGNAL(key_tap(double,double,int)), operation(), SLOT());
    connect(this, SIGNAL(key_tap(int)), operation(), SLOT());
    connect(this, SIGNAL(grab(int)), operation(), SLOT());
    connect(this, SIGNAL(grab(double,double)), operation(), SLOT());
    connect(this, SIGNAL(grab(double,double,int)), operation(), SLOT());
    connect(this, SIGNAL(pinch(int)), operation(), SLOT());
    connect(this, SIGNAL(pinch(double,double)), operation(), SLOT());
    connect(this, SIGNAL(pinch(double,double,int)), operation(), SLOT());
    */
}

void Process::changedToExplorer()
{
    disconnectAll();
    QVariantMap config_operation_explorer = config_operation["explorer"].toMap();
    QVariantMap config_operation_explorer_dial = config_operation_explorer["dial"].toMap();
    if(config_operation_explorer_dial["up"].toString() == "gonext"){
        connect(display.widgetDial(), SIGNAL(choose_up(double, double)), operation, SLOT(openSometing(double, double)));
    }else{
        connect(display.widgetDial(), SIGNAL(choose_up(QString)), operation, SLOT(execProgram(QString)));
    }
    if(config_operation_explorer_dial["down"].toString() == "goback"){
        connect(display.widgetDial(), SIGNAL(choose_down()), operation, SLOT(goBack()));
    }else{
        connect(display.widgetDial(), SIGNAL(choose_down(QString)), operation, SLOT(execProgram(QString)));
    }
    if(config_operation_explorer_dial["left"].toString() == "fresh"){
        connect(display.widgetDial(), SIGNAL(choose_left()), operation, SLOT(goRefresh()));
    }else{
        connect(display.widgetDial(), SIGNAL(choose_left(QString)), operation, SLOT(execProgram(QString)));
    }
    if(config_operation_explorer_dial["right"].toString() == "desktop"){
        connect(display.widgetDial(), SIGNAL(choose_right()), operation, SLOT(showDesktop()));
    }else{
        connect(display.widgetDial(), SIGNAL(choose_right(QString)), operation, SLOT(execProgram(QString)));
    }
    /*
    connect(this, SIGNAL(swipe(int)), operation(), SLOT());
    connect(this, SIGNAL(circle(int)), operation(), SLOT());
    connect(this, SIGNAL(circle_anti(int)), operation(), SLOT());
    connect(this, SIGNAL(screen_tap(double,double,int)), operation(), SLOT());
    connect(this, SIGNAL(screen_tap(int)), operation(), SLOT());
    connect(this, SIGNAL(key_tap(double,double,int)), operation(), SLOT());
    connect(this, SIGNAL(key_tap(int)), operation(), SLOT());
    connect(this, SIGNAL(grab(int)), operation(), SLOT());
    connect(this, SIGNAL(grab(double,double)), operation(), SLOT());
    connect(this, SIGNAL(grab(double,double,int)), operation(), SLOT());
    connect(this, SIGNAL(pinch(int)), operation(), SLOT());
    connect(this, SIGNAL(pinch(double,double)), operation(), SLOT());
    connect(this, SIGNAL(pinch(double,double,int)), operation(), SLOT());
    */
}

void Process::changedToBrowser()
{
    disconnectAll();
    /*
    connect(this, SIGNAL(swipe(int)), operation(), SLOT());
    connect(this, SIGNAL(circle(int)), operation(), SLOT());
    connect(this, SIGNAL(circle_anti(int)), operation(), SLOT());
    connect(this, SIGNAL(screen_tap(double,double,int)), operation(), SLOT());
    connect(this, SIGNAL(screen_tap(int)), operation(), SLOT());
    connect(this, SIGNAL(key_tap(double,double,int)), operation(), SLOT());
    connect(this, SIGNAL(key_tap(int)), operation(), SLOT());
    connect(this, SIGNAL(grab(int)), operation(), SLOT());
    connect(this, SIGNAL(grab(double,double)), operation(), SLOT());
    connect(this, SIGNAL(grab(double,double,int)), operation(), SLOT());
    connect(this, SIGNAL(pinch(int)), operation(), SLOT());
    connect(this, SIGNAL(pinch(double,double)), operation(), SLOT());
    connect(this, SIGNAL(pinch(double,double,int)), operation(), SLOT());
    */
}

void Process::changedToOther()
{
    disconnectAll();
    /*
    connect(this, SIGNAL(swipe(int)), operation(), SLOT());
    connect(this, SIGNAL(circle(int)), operation(), SLOT());
    connect(this, SIGNAL(circle_anti(int)), operation(), SLOT());
    connect(this, SIGNAL(screen_tap(double,double,int)), operation(), SLOT());
    connect(this, SIGNAL(screen_tap(int)), operation(), SLOT());
    connect(this, SIGNAL(key_tap(double,double,int)), operation(), SLOT());
    connect(this, SIGNAL(key_tap(int)), operation(), SLOT());
    connect(this, SIGNAL(grab(int)), operation(), SLOT());
    connect(this, SIGNAL(grab(double,double)), operation(), SLOT());
    connect(this, SIGNAL(grab(double,double,int)), operation(), SLOT());
    connect(this, SIGNAL(pinch(int)), operation(), SLOT());
    connect(this, SIGNAL(pinch(double,double)), operation(), SLOT());
    connect(this, SIGNAL(pinch(double,double,int)), operation(), SLOT());
    */
}

void Process::disconnectAll()
{
    /*
    disconnect(this, SIGNAL(swipe(int)), operation(), 0);
    disconnect(this, SIGNAL(circle(int)), operation(), 0);
    disconnect(this, SIGNAL(circle_anti(int)), operation(), 0);
    disconnect(this, SIGNAL(screen_tap(double,double,int)), operation(), 0);
    disconnect(this, SIGNAL(screen_tap(int)), operation(), 0);
    disconnect(this, SIGNAL(key_tap(double,double,int)), operation(), 0);
    disconnect(this, SIGNAL(key_tap(int)), operation(), 0);
    disconnect(this, SIGNAL(grab(int)), operation(), 0);
    disconnect(this, SIGNAL(grab(double,double)), operation(), 0);
    disconnect(this, SIGNAL(grab(double,double,int)), operation(), 0);
    disconnect(this, SIGNAL(pinch(int)), operation(), 0);
    disconnect(this, SIGNAL(pinch(double,double)), operation(), 0);
    disconnect(this, SIGNAL(pinch(double,double,int)), operation(), 0);
    */
}

QString Process::getForegroundWindow(){
    
    Foreground foreground_window;
    QString foreground_window_name = QString::fromStdString(foreground_window.getForegroundWindowName());
    QString foreground_window_process_name = QString::fromStdString(foreground_window.getForegroundProcessName());
    if(foreground_window_process_name.contains("firefox.exe",Qt::CaseSensitive) ||
    foreground_window_process_name.contains("chrome.exe",Qt::CaseSensitive) ||
    foreground_window_process_name.contains("iexplore.exe",Qt::CaseSensitive) ||
    foreground_window_process_name.contains("opera.exe",Qt::CaseSensitive)
    ){
        return "browser";
    }
    if(foreground_window_process_name.contains("explorer.exe",Qt::CaseSensitive)){
        if(foreground_window_name.contains("Program Manager",Qt::CaseSensitive)){
            return "desktop";
        }else{
            return "explorer";
        }
    }
    return "other";
    
}

void Process::dial_up(){
    QVariantMap config_operation = config_operation[getForegroundWindow()].toMap();
    QVariantMap config_operation_dial = config_operation["dial"].toMap();
    emit choose_up(config_operation_dial["up"].toString)
}

void Process::dial_down(){
    QVariantMap config_operation = config_operation[getForegroundWindow()].toMap();
    QVariantMap config_operation_dial = config_operation["dial"].toMap();
    emit choose_down(config_operation_dial["down"].toString)
}

void Process::dial_left(){
    QVariantMap config_operation = config_operation[getForegroundWindow()].toMap();
    QVariantMap config_operation_dial = config_operation["dial"].toMap();
    emit choose_left(config_operation_dial["left"].toString)
}
void Process::dial_right(){
    QVariantMap config_operation = config_operation[getForegroundWindow()].toMap();
    QVariantMap config_operation_dial = config_operation["dial"].toMap();
    emit choose_right(config_operation_dial["right"].toString)
}