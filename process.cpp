#include "process.h"
#include "motion_code.h"
#include "foreground.h"
#include <QJsonDocument>
#include <QDate>
#include <QCoreApplication>
Process::Process(QObject *parent) : QObject(parent)
{
    loop_timer = new QTimer(this);
    operation = new Operation(this);
    
    if(!config["log"].isNull()){
        QDate date;
        QString current_date = date.currentDate().toString("yyyy-MM-dd");
        QString log_file_name = current_date + ".log";
        logger.setLogDir(config["log"].toString());
        logger.setLogFile(config["log"].toString() + "\\" + log_file_name);
    }
    

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
    
    logger.log("INFO", "Load the config file: " + config_file.fileName());

    if(connect(this, SIGNAL(setGesture(QString)), this, SLOT(showGesture(QString)))){
        logger.log("INFO", "Connected gesture show signal/slot.");
    }else{
        logger.log("ERROR", "Failed to connect gesture show signal/slot.");
    }
}

void Process::init(int argc, char* argv[])
{
    
    if(config_display["palm"].toString() == "true"){
        display.showPalm();
        logger.log("INFO", "Set the Palm Display Widget visible.");
    }else if(config_display["palm"].toString() == "false"){
        display.hidePalm();
        logger.log("INFO", "Set the Palm Display Widget unvisible.");
    }
    if(config_display["gesture"].toString() == "true"){
        display.showGesture();
        logger.log("INFO", "Set the Gesture Display Widget visible.");
    }else if(config_display["gesture"].toString() == "false"){
        display.hideGesture();
        logger.log("INFO", "Set the Gesture Display Widget unvisible.");
    }
    if(config_display["status"].toString() == "true"){
        display.showStatus();
        logger.log("INFO", "Set the Status Display Widget visible.");
    }else if(config_display["status"].toString() == "false"){
        display.hideStatus();
        logger.log("INFO", "Set the Status Display Widget visible.");
    }
    for(int arg = 1; arg < argc; arg ++){
        if(argv[arg] == "-d"){
            display.showStatus();
            logger.log("INFO", "The application execute with \"-d\", set the Status Display Widget visible.");
        }else if(argv[arg] == "-s"){
            display.hideGesture();
            logger.log("INFO", "The application execute with \"-s\", set the Gesture Display Widget unvisible.");
        }else if(argv[arg] == "-h"){
            display.showPalm();
            logger.log("INFO", "The application execute with \"-h\", set the Palm Display Widget visible.");
        }else{
            logger.log("ERROR", "The application execute with wrong parameter.");
        }
    }
    
    if(config["controller"].toString() == "LEAPMOTION"){
        motion.setController(LEAP_MOTION);
            logger.log("INFO", "The controller is set to LEAP MOTION.");
    }
    
    if(config["handedness"].toString() == "left"){
        motion.setHandedness(true);
        logger.log("INFO", "Handedness is set to left.");
    }else if(config["handedness"].toString() == "right"){
        motion.setHandedness(false);
        logger.log("INFO", "Handedness is set to right.");
    }
    
    logger.log("INFO", "Process initialized.");


}

void Process::start()
{
    connect(loop_timer,SIGNAL(timeout()),this,SLOT(run())))
    loop_timer->start(20);
    logger.log("INFO", "Set run() frequency to one time per 20ms.");
}

void Process::run()
{
    motion.update();
       
    if(motion.getHandCount() == 0){
        emit setGesture("off");
    }else{
        display.updateCursor(motion.getNormalizedX(), motion.getNormalizedY());
        display.updateStatus(motion.getControllerStatus()*4 + motion.getServiceStatus()*2 + motion.getProcessStatus());
        if(foreground_prev != getForegroundWindow()){
            if(getForegroundWindow() == "desktop"){
                emit changedToDesktop();
                logger.log("INFO", "Foreground changed to Desktop.");
            }
            if(getForegroundWindow() == "explorer"){
                emit changedToExplorer();
                logger.log("INFO", "Foreground changed to Explorer.");
            }
            if(getForegroundWindow() == "browser"){
                emit changedToBrowser();
                logger.log("INFO", "Foreground changed to Browser.");
            }
            if(getForegroundWindow() == "other"){
                emit changedToOther();
                 logger.log("INFO", "Foreground changed to Other.");
           }
            foreground_prev = getForegroundWindow();
        }
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
                        logger.log("INFO", "Gesture Grab with 2/2 hands start.");
                        logKeyInfo();
                        break;
                    case KEEP:
                        emit grab(KEEP);
                        emit setGesture("grab");
                        break;
                    case STOP:
                        emit grab(STOP);
                        emit setGesture("hand");
                        logger.log("INFO", "Gesture Grab with 2/2 hands stop.");
                        logKeyInfo();
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
                        logger.log("INFO", "Gesture Grab with 1/2 hands start.");
                        logKeyInfo();
                        break;
                    case KEEP:
                        emit grab(motion.getNormalizedX(), motion.getNormalizedY());
                        emit setGesture("grab");
                        break;
                    case STOP:
                        emit grab(motion.getNormalizedX(), motion.getNormalizedY(), 0);
                        emit setGesture("hand");
                         logger.log("INFO", "Gesture Grab with 1/2 hands start.");
                         logKeyInfo();
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
                        logger.log("INFO", "Gesture Pinch with 2/2 hands start.");
                        logKeyInfo();
                        break;
                    case KEEP:
                        emit pinch(KEEP);
                        emit setGesture("pinch");
                        break;
                    case STOP:
                        emit pinch(STOP);
                        emit setGesture("pinche");
                        logger.log("INFO", "Gesture Pinch with 2/2 hands start.");
                        logKeyInfo();
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
                        emit pinch(motion.getNormalizedX(), motion.getNormalizedY(), 1);
                        emit setGesture("pinch");
                        logger.log("INFO", "Gesture Pinch with 1/2 hands start.");
                        logKeyInfo();
                        break;
                    case KEEP:
                        emit pinch(motion.getNormalizedX(), motion.getNormalizedY());
                        emit setGesture("pinch");
                        break;
                    case STOP:
                        emit pinch(motion.getNormalizedX(), motion.getNormalizedY(), 0);
                        emit setGesture("pinche");
                        logger.log("INFO", "Gesture Pinch with 1/2 hands start.");
                        logKeyInfo();
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
                        emit grab(motion.getNormalizedX(), motion.getNormalizedY(), 1);
                        emit setGesture("grab");
                        logger.log("INFO", "Gesture Grab start.");
                        logKeyInfo();
                        break;
                    case KEEP:
                        emit grab(motion.getNormalizedX(), motion.getNormalizedY());
                        emit setGesture("grab");
                        break;
                    case STOP:
                        emit grab(motion.getNormalizedX(), motion.getNormalizedY(), 0);
                        emit setGesture("hand");
                        logger.log("INFO", "Gesture Grab start.");
                        logKeyInfo();
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
                        emit pinch(motion.getNormalizedX(), motion.getNormalizedY(), 1);
                        emit setGesture("pinch");
                        emit showDial(true);
                        emit showDial(motion.getNormalizedX(), motion.getNormalizedY());
                        logger.log("INFO", "Gesture Pinch start.");
                        logger.log("INFO", "Start Position: " + motion.getNormalizedX() + ", " + motion.getNormalizedY());
                        logKeyInfo();
                        break;
                    case KEEP:
                        emit pinch(motion.getNormalizedX(), motion.getNormalizedY());
                        emit setGesture("pinch");
                        emit updateDial(motion.getNormalizedX(), motion.getNormalizedY());
                        break;
                    case STOP:
                        emit pinch(motion.getNormalizedX(), motion.getNormalizedY(), 0);
                        emit setGesture("hand");
                        emit showDial(false);
                        emit doneDial();
                        logger.log("INFO", "Gesture Pinch stop.");
                        logger.log("INFO", "Stop Position: " + motion.getNormalizedX() + ", " + motion.getNormalizedY());
                        logKeyInfo();
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
                        emit circle(5);
                        emit setGesture("circle");
                        logger.log("INFO", "Gesture Circle start.");
                        logKeyInfo();
                        break;
                    case KEEP:
                        emit circle(5);
                        emit setGesture("circle");
                        break;
                    case STOP:
                        emit circle(0);
                        emit setGesture("hand");
                        logger.log("INFO", "Gesture Circle stop.");
                        logKeyInfo();
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
                        emit circle_anti(-5);
                        emit setGesture("circle_anti");
                        logger.log("INFO", "Gesture Circle Anti start.");
                        logKeyInfo();
                        break;
                    case KEEP:
                        emit circle_anti(-5);
                        emit setGesture("circle_anti");
                        break;
                    case STOP:
                        emit circle_anti(0);
                        emit setGesture("hand");
                        logger.log("INFO", "Gesture Circle Anti stop.");
                        logKeyInfo();
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
                        logger.log("INFO", "Gesture Swipe start.");
                        logKeyInfo();
                        break;
                    case KEEP:
                        emit swipe(KEEP);
                        emit setGesture("swipe");
                        break;
                    case STOP:
                        emit swipe(STOP);
                        emit setGesture("hand");
                        logger.log("INFO", "Gesture Swipe stop.");
                        logKeyInfo();
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
                        logger.log("INFO", "Gesture Screen Tap start.");
                        logKeyInfo();
                        break;
                    case KEEP:
                        emit screen_tap(KEEP);
                        break;
                    case STOP:
                        emit screen_tap(STOP);
                        logger.log("INFO", "Gesture Screen Tap stop.");
                        logKeyInfo();
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
                        logger.log("INFO", "Gesture Key Tap start.");
                        logKeyInfo();
                        break;
                    case KEEP:
                        emit key_tap(KEEP);
                        break;
                    case STOP:
                        emit key_tap(STOP);
                        logger.log("INFO", "Gesture Key Tap stop.");
                        logKeyInfo();
                        break;
                    default:
                        break;
                }
            }
        }
    }
}
void Process::stop()
{
    disconnect(loop_timer,SIGNAL(timeout()),this,SLOT(run()));
    logger.log("INFO", "Process stoped.");
}

void Process::restart()
{
    stop();
    logger.log("INFO", "Process stoped.");
    start();
    logger.log("INFO", "Process started.");
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
    connect(this, SIGNAL(showDial(bool)), display.widgetDial(), SLOT(setAvailable(bool)));
    connect(this, SIGNAL(showDial(double, double)), display.widgetDial(), SLOT(setPosition(double, double)));
    connect(this, SIGNAL(updateDial(double, double)), display.widgetDial(), SLOT(setTargetPosition(double, double)));
    connect(this, SIGNAL(doneDial()), display.widgetDial(), SLOT(doneDial()));

    if(config_operation_desktop_dial["up"].toString() == "opensometing"){
        connect(display.widgetDial(), SIGNAL(choose_up(double, double)), operation, SLOT(openSomething(double, double)));
    }else{
        connect(this, SIGNAL(choose_up(QString)), operation, SLOT(execProgram(QString)));
    }
    if(config_operation_desktop_dial["down"].toString() == "setting"){
        connect(display.widgetDial(), SIGNAL(choose_down()), operation, SLOT(lockscreen()));
    }else{
        connect(this, SIGNAL(choose_down(QString)), operation, SLOT(execProgram(QString)));
    }
    if(config_operation_desktop_dial["left"].toString() == "browser"){
        connect(display.widgetDial(), SIGNAL(choose_left()), operation, SLOT(openBrowser()));
    }else{
        connect(this, SIGNAL(choose_left(QString)), operation, SLOT(execProgram(QString)));
    }
    if(config_operation_desktop_dial["right"].toString() == "explorer"){
        connect(display.widgetDial(), SIGNAL(choose_right()), operation, SLOT(openExplorer()));
    }else{
        connect(this, SIGNAL(choose_right(QString)), operation, SLOT(execProgram(QString)));
    }
    connect(this, SIGNAL(pinch(double, double, bool)), operation, SLOT(mouseSelect(double, double, bool)));
    logger.log("INFO", "Re-connected signals/slots in foreground Desktop.");
}

void Process::changedToExplorer()
{
    disconnectAll();
    QVariantMap config_operation_explorer = config_operation["explorer"].toMap();
    QVariantMap config_operation_explorer_dial = config_operation_explorer["dial"].toMap();

    connect(this, SIGNAL(showDial(bool)), display.widgetDial(), SLOT(setAvailable(bool)));
    connect(this, SIGNAL(showDial(double, double)), display.widgetDial(), SLOT(setPosition(double, double)));
    connect(this, SIGNAL(updateDial(double, double)), display.widgetDial(), SLOT(setTargetPosition(double, double)));
    connect(this, SIGNAL(doneDial()), display.widgetDial(), SLOT(doneDial()));


    if(config_operation_explorer_dial["up"].toString() == "gonext"){
        connect(display.widgetDial(), SIGNAL(choose_up(double, double)), operation, SLOT(openSomething(double, double)));
    }else{
        connect(this, SIGNAL(choose_up(QString)), operation, SLOT(execProgram(QString)));
    }
    if(config_operation_explorer_dial["down"].toString() == "goback"){
        connect(display.widgetDial(), SIGNAL(choose_down()), operation, SLOT(goBack()));
    }else{
        connect(this, SIGNAL(choose_down(QString)), operation, SLOT(execProgram(QString)));
    }
    if(config_operation_explorer_dial["left"].toString() == "fresh"){
        connect(display.widgetDial(), SIGNAL(choose_left()), operation, SLOT(goRefresh()));
    }else{
        connect(this, SIGNAL(choose_left(QString)), operation, SLOT(execProgram(QString)));
    }
    if(config_operation_explorer_dial["right"].toString() == "desktop"){
        connect(display.widgetDial(), SIGNAL(choose_right()), operation, SLOT(showDesktop()));
    }else{
        connect(this, SIGNAL(choose_right(QString)), operation, SLOT(execProgram(QString)));
    }
    connect(this, SIGNAL(circle(int)), operation, SLOT(mouseRoll(int)));
    connect(this, SIGNAL(circle_anti(int)), operation, SLOT(mouseRoll(int)));
    connect(this, SIGNAL(pinch(double, double, bool)), operation, SLOT(mouseSelect(double, double, bool)));
    logger.log("INFO", "Re-connected signals/slots in foreground Explorer.");
}

void Process::changedToBrowser()
{
    disconnectAll();
    connect(this, SIGNAL(circle(int)), operation, SLOT(mouseRoll(int)));
    connect(this, SIGNAL(circle_anti(int)), operation, SLOT(mouseRoll(int)));
    connect(this, SIGNAL(grab(int)), operation, SLOT(swipeBrowserTab(int)));
    connect(this, SIGNAL(pinch(double, double, bool)), operation, SLOT(mouseSelect(double, double, bool)));
    logger.log("INFO", "Re-connected signals/slots in foreground Browser.");
}

void Process::changedToOther()
{
    disconnectAll();
    connect(this, SIGNAL(pinch(double, double, bool)), operation, SLOT(mouseSelect(double, double, bool)));
    logger.log("INFO", "Re-connected signals/slots in foreground Other.");
}

void Process::connectToDial()
{
    connect(this, SIGNAL(showDial(bool)), display.widgetDial(), SLOT(setAvailable(bool)));
    connect(this, SIGNAL(showDial(double, double)), display.widgetDial(), SLOT(setPosition(double, double)));
    connect(this, SIGNAL(updateDial(double, double)), display.widgetDial(), SLOT(setTargetPosition(double, double)));
    connect(this, SIGNAL(doneDial()), display.widgetDial(), SLOT(doneDial()));
    connect(display.widgetDial(), SIGNAL(choose_up()), this, SLOT(dial_up()));
    connect(display.widgetDial(), SIGNAL(choose_down()), this, SLOT(dial_down()));
    connect(display.widgetDial(), SIGNAL(choose_left()), this, SLOT(dial_left()));
    connect(display.widgetDial(), SIGNAL(choose_right()), this, SLOT(dial_right()));
}

void Process::disconnectToDial()
{
    disconnect(this, SIGNAL(showDial(bool)), 0, 0);
    disconnect(this, SIGNAL(showDial(double, double)), 0, 0);
    disconnect(this, SIGNAL(updateDial(double, double)), 0, 0);
    disconnect(this, SIGNAL(doneDial()), 0, 0);
    disconnect(display.widgetDial(), SIGNAL(choose_up()), 0, 0);
    disconnect(display.widgetDial(), SIGNAL(choose_down()), 0, 0);
    disconnect(display.widgetDial(), SIGNAL(choose_left()), 0, 0);
    disconnect(display.widgetDial(), SIGNAL(choose_right()), 0, 0);
}

void Process::disconnectAll()
{
    disconnect(this, SIGNAL(pinch(double, double, bool)), 0, 0);
    disconnect(this, SIGNAL(grab(int)), 0, 0);
    disconnect(this, SIGNAL(circle(int)), 0, 0);
    disconnect(this, SIGNAL(circle_anti(int)), 0, 0);
    disconnect(display.widgetDial(), SIGNAL(choose_up(double, double)), operation, 0);
    disconnect(this, SIGNAL(choose_up(QString)), operation, 0);
    disconnect(display.widgetDial(), SIGNAL(choose_down(double, double)), operation, 0);
    disconnect(this, SIGNAL(choose_down(QString)), operation, 0);
    disconnect(display.widgetDial(), SIGNAL(choose_left(double, double)), operation, 0);
    disconnect(this, SIGNAL(choose_left(QString)), operation, 0);
    disconnect(display.widgetDial(), SIGNAL(choose_right(double, double)), operation, 0);
    disconnect(this, SIGNAL(choose_right(QString)), operation, 0);
    disconnectToDial();
    logger.log("INFO", "Disconnected All signals and slots between gesture/userevent an operation.");
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
    emit choose_up(config_operation_dial["up"].toString());
    logger.log("INFO", "Dial choose to UP.");
}

void Process::dial_down(){
    QVariantMap config_operation = config_operation[getForegroundWindow()].toMap();
    QVariantMap config_operation_dial = config_operation["dial"].toMap();
    emit choose_down(config_operation_dial["down"].toString());
    logger.log("INFO", "Dial choose to DOWN.");
}

void Process::dial_left(){
    QVariantMap config_operation = config_operation[getForegroundWindow()].toMap();
    QVariantMap config_operation_dial = config_operation["dial"].toMap();
    emit choose_left(config_operation_dial["left"].toString());

    logger.log("INFO", "Dial choose to LEFT.");
}
void Process::dial_right(){
    QVariantMap config_operation = config_operation[getForegroundWindow()].toMap();
    QVariantMap config_operation_dial = config_operation["dial"].toMap();
    emit choose_right(config_operation_dial["right"].toString());
    logger.log("INFO", "Dial choose to RIGHT.");
}

void Process::logKeyInfo(){
    logger.log("INFO", "===================== KEY INFORMATION =====================");
    logger.log("INFO", "Controller Status: " + QString::number(motion.getControllerStatus()) + ", Service Status: " + QString::number(motion.getServiceStatus()) + ", Process Status: " + QString::number(motion.getProcessStatus()));
    logger.log("INFO", "Position: x: " + motion.getNormalizedX() + ", y: " + motion.getNormalizedY());
    logger.log("INFO", "Hands Cout: " + motion.getHandCount());
    logger.log("INFO", "===================== KEY INFORMATION =====================");
}