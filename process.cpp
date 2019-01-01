//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2019 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


#include <QJsonDocument>
#include <QDate>
#include <QCoreApplication>
#include "process.h"
#include "motion_code.h"
#include "foreground.h"

Process::Process(QObject *parent) : QObject(parent){

    loop_timer = new QTimer(this);
    operation = new Operation(this);
    
    //load the config.json
    QString config_sring;
    QFile config_file;
    config_file.setFileName(":/config.json");
    //set QIODevice RO and read as text
    config_file.open(QIODevice::ReadOnly | QIODevice::Text);
    config_sring = config_file.readAll();
    config_file.close();

    //convert file to variant map
    QJsonDocument config_json = QJsonDocument::fromJson(config_sring.toUtf8());
    config = config_json.toVariant().toMap();
    //save gesture config in another variant map
    config_gesture = config["gesture"].toMap();
    //save display config in another variant map
    config_display = config["display"].toMap();
    //save operation config in another variant map
    config_operation = config["operation"].toMap();
    //save config in the same variant map
    config = config["config"].toMap();
    
    //if log file had setup in config.json
    if(!config["log"].isNull()){
        QDate date;
        //log file name is cureent date
        QString current_date = date.currentDate().toString("yyyy-MM-dd");
        QString log_file_name = current_date + ".log";
        //use the path in json
        logger.setLogDir(config["log"].toString());
        logger.setLogFile(config["log"].toString() + "\\" + log_file_name);
    }

    logger.log("INFO", "Loaded the config file: " + config_file.fileName());

}

void Process::init(int argc, char* argv[]){


    //set Palm Widget available or not
    if(config_display["palm"].toString() == "true"){
        display.showPalm();
        logger.log("INFO", "Set the Palm Display Widget visible.");
    }else if(config_display["palm"].toString() == "false"){
        display.hidePalm();
        logger.log("INFO", "Set the Palm Display Widget unvisible.");
    }

    //set Gesture Widget available or not
    if(config_display["gesture"].toString() == "true"){
        display.showGesture();
        logger.log("INFO", "Set the Gesture Display Widget visible.");
    }else if(config_display["gesture"].toString() == "false"){
        display.hideGesture();
        logger.log("INFO", "Set the Gesture Display Widget unvisible.");
    }

    //set Status Widget available or not
    if(config_display["status"].toString() == "true"){
        display.showStatus();
        logger.log("INFO", "Set the Status Display Widget visible.");
    }else if(config_display["status"].toString() == "false"){
        display.hideStatus();
        logger.log("INFO", "Set the Status Display Widget visible.");
    }

    //application parameter: -d for debug, -s for silence, -h for palm show
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

    //if controller type is LEAPMOTION
    if(config["controller"].toString() == "LEAPMOTION"){
        motion.setController(LEAP_MOTION);
            logger.log("INFO", "The controller is set to LEAP MOTION.");
    }
    
    //set handedness according to the value in config
    if(config["handedness"].toString() == "left"){
        motion.setHandedness(true);
        logger.log("INFO", "Handedness is set to left.");
    }else if(config["handedness"].toString() == "right"){
        motion.setHandedness(false);
        logger.log("INFO", "Handedness is set to right.");
    }
    
    //connect gesture show signal/slot
    if(connect(this, SIGNAL(setGesture(QString)), this, SLOT(showGesture(QString)))){
        logger.log("INFO", "Connected gesture show signal/slot.");
    }else{
        logger.log("ERROR", "Failed to connect gesture show signal/slot.");
    }

    logger.log("INFO", "Process initialized.");

}

void Process::start(){

    //start run()
    connect(loop_timer, SIGNAL(timeout()), this, SLOT(run()));
    loop_timer->start(20);
    logger.log("INFO", "Set run() frequency to one time per 20ms.");

}

void Process::run(){

    //update motion
    motion.update();

    if(motion.getHandCount() == 0){

        //no hand in frame
        emit setGesture("off");
    }else{

        //update display and status
        display.updateCursor(motion.getNormalizedX(), motion.getNormalizedY());
        display.updateStatus(motion.getControllerStatus()*4 + motion.getServiceStatus()*2 + motion.getProcessStatus());

        //get foreground and know is`t the same as before
        if(foreground_prev != getForegroundWindow()){

            //desktop
            if(getForegroundWindow() == "desktop"){
                emit changedToDesktop();
                logger.log("INFO", "Foreground changed to Desktop.");
            }

            //explorer
            if(getForegroundWindow() == "explorer"){
                emit changedToExplorer();
                logger.log("INFO", "Foreground changed to Explorer.");
            }

            //browser
            if(getForegroundWindow() == "browser"){
                emit changedToBrowser();
                logger.log("INFO", "Foreground changed to Browser.");
            }

            //other
            if(getForegroundWindow() == "other"){
                emit changedToOther();
                 logger.log("INFO", "Foreground changed to Other.");
           }

           //save the previous foreground
           foreground_prev = getForegroundWindow();
        }

        //set the picture dial show
        display.widgetDial()->setDial(getForegroundWindow());

        //load the runtime config
        QVariantMap config_runtime = config_gesture[getForegroundWindow()].toMap();

        //get gesture status
        if(motion.getHandCount() == 2){

            //2 hands gesture

            //gesture: 2 hands grab
            if(config_runtime["grab_2hands"].toString() == "true"){

                //get 2 hands grab status
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

            //gesture: 1/2 hands grab
            if(config_runtime["grab"].toString() == "true"){

                //get hands grab status
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

            //gesture: 2 hands pinch
            if(config_runtime["pinch_2hands"].toString() == "true"){

                //get 2 hands pinch status
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

            //gesture: 1/2 hands grab
            if(config_runtime["pinch"] == "true"){
                //get hands pinch status
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
            //1 hand gesture

            //gesture: hand grab
            if(config_runtime["grab"].toString() == "true"){
                //get hand grab status
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

            //gesture: hand pinch
            if(config_runtime["pinch"].toString() == "true"){
                //get hand pinch status
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
                        logger.log("INFO", "Start Position: " + QString::number(motion.getNormalizedX()) + ", " + QString::number(motion.getNormalizedY()));
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
                        logger.log("INFO", "Stop Position: " + QString::number(motion.getNormalizedX()) + ", " + QString::number(motion.getNormalizedY()));
                        logKeyInfo();
                        break;
                    default:
                        break;
                }
            }

            //gesture: hand circle
            if(config_runtime["circle"].toString() == "true"){
                //get hand circle status
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

            //gesture: hand circle-anti
            if(config_runtime["circle_anti"].toString() == "true"){
                //get hand circle-anti status
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

            //gesture: hand swipe
            if(config_runtime["swipe"].toString() == "true"){
                //get hand swipe status
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

            //gesture: hand screen tap
            if(config_runtime["screen_tap"].toString() == "true"){
                //get hand screen tap status
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

            //gesture: hand key tap
            if(config_runtime["key_tap"].toString() == "true"){
                //get hand key tap status
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
void Process::stop(){

    //disconnect the timer and run(), stop running
    disconnect(loop_timer,SIGNAL(timeout()),this,SLOT(run()));
    logger.log("INFO", "Process stoped.");
}

void Process::restart(){

    //stop() and then start()
    stop();
    logger.log("INFO", "Process stoped.");
    start();
    logger.log("INFO", "Process started.");
}

void Process::showGesture(QString gesture_type){

    //show the gesture
    display.showGesture();
    display.setGestureDisplay(gesture_type);
}

void Process::changedToDesktop(){

    //disconnect all signals/slot in previous foreground
    disconnectAll();

    //load desktop dial config
    QVariantMap config_operation_desktop = config_operation["desktop"].toMap();
    QVariantMap config_operation_desktop_dial = config_operation_desktop["dial"].toMap();

    //connect the dial signals/slots in desktop
    connect(this, SIGNAL(showDial(bool)), display.widgetDial(), SLOT(setAvailable(bool)));
    connect(this, SIGNAL(showDial(double, double)), display.widgetDial(), SLOT(setPosition(double, double)));
    connect(this, SIGNAL(updateDial(double, double)), display.widgetDial(), SLOT(setTargetPosition(double, double)));
    connect(this, SIGNAL(doneDial()), display.widgetDial(), SLOT(doneDial()));

    //connect the dial choose event

    //choose up
    if(config_operation_desktop_dial["up"].toString() == "opensometing"){
        connect(display.widgetDial(), SIGNAL(choose_up(double, double)), operation, SLOT(openSomething(double, double)));
    }else{
        connect(this, SIGNAL(choose_up(QString)), operation, SLOT(execProgram(QString)));
    }

    //choose down
    if(config_operation_desktop_dial["down"].toString() == "setting"){
        connect(display.widgetDial(), SIGNAL(choose_down()), operation, SLOT(lockscreen()));
    }else{
        connect(this, SIGNAL(choose_down(QString)), operation, SLOT(execProgram(QString)));
    }

    //choose left
    if(config_operation_desktop_dial["left"].toString() == "browser"){
        connect(display.widgetDial(), SIGNAL(choose_left()), operation, SLOT(openBrowser()));
    }else{
        connect(this, SIGNAL(choose_left(QString)), operation, SLOT(execProgram(QString)));
    }

    //choose right
    if(config_operation_desktop_dial["right"].toString() == "explorer"){
        connect(display.widgetDial(), SIGNAL(choose_right()), operation, SLOT(openExplorer()));
    }else{
        connect(this, SIGNAL(choose_right(QString)), operation, SLOT(execProgram(QString)));
    }

    //connect the pinch/select
    connect(this, SIGNAL(pinch(double, double, bool)), operation, SLOT(mouseSelect(double, double, bool)));

    logger.log("INFO", "Re-connected signals/slots in foreground Desktop.");
}

void Process::changedToExplorer(){

    //disconnect all signals/slot in previous foreground
    disconnectAll();
    QVariantMap config_operation_explorer = config_operation["explorer"].toMap();
    QVariantMap config_operation_explorer_dial = config_operation_explorer["dial"].toMap();

    //connect dial signals/slots
    connect(this, SIGNAL(showDial(bool)), display.widgetDial(), SLOT(setAvailable(bool)));
    connect(this, SIGNAL(showDial(double, double)), display.widgetDial(), SLOT(setPosition(double, double)));
    connect(this, SIGNAL(updateDial(double, double)), display.widgetDial(), SLOT(setTargetPosition(double, double)));
    connect(this, SIGNAL(doneDial()), display.widgetDial(), SLOT(doneDial()));

    //connect choose signals/slots
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

    //connect gesture signals/slots
    connect(this, SIGNAL(circle(int)), operation, SLOT(mouseRoll(int)));
    connect(this, SIGNAL(circle_anti(int)), operation, SLOT(mouseRoll(int)));
    connect(this, SIGNAL(pinch(double, double, bool)), operation, SLOT(mouseSelect(double, double, bool)));

    logger.log("INFO", "Re-connected signals/slots in foreground Explorer.");
}

void Process::changedToBrowser(){

    //disconnect all signals/slot in previous foreground
    disconnectAll();

    //connect gesture signals/slots
    connect(this, SIGNAL(circle(int)), operation, SLOT(mouseRoll(int)));
    connect(this, SIGNAL(circle_anti(int)), operation, SLOT(mouseRoll(int)));
    connect(this, SIGNAL(grab(int)), operation, SLOT(swipeBrowserTab(int)));
    connect(this, SIGNAL(pinch(double, double, bool)), operation, SLOT(mouseSelect(double, double, bool)));

    logger.log("INFO", "Re-connected signals/slots in foreground Browser.");
}

void Process::changedToOther(){

    //disconnect all signals/slot in previous foreground
    disconnectAll();

    //connect the pinch/select signals/slots
    connect(this, SIGNAL(pinch(double, double, bool)), operation, SLOT(mouseSelect(double, double, bool)));

    logger.log("INFO", "Re-connected signals/slots in foreground Other.");
}

void Process::connectToDial(){

    //connect dial signals/slots
    connect(this, SIGNAL(showDial(bool)), display.widgetDial(), SLOT(setAvailable(bool)));
    connect(this, SIGNAL(showDial(double, double)), display.widgetDial(), SLOT(setPosition(double, double)));
    connect(this, SIGNAL(updateDial(double, double)), display.widgetDial(), SLOT(setTargetPosition(double, double)));
    connect(this, SIGNAL(doneDial()), display.widgetDial(), SLOT(doneDial()));
    connect(display.widgetDial(), SIGNAL(choose_up()), this, SLOT(dial_up()));
    connect(display.widgetDial(), SIGNAL(choose_down()), this, SLOT(dial_down()));
    connect(display.widgetDial(), SIGNAL(choose_left()), this, SLOT(dial_left()));
    connect(display.widgetDial(), SIGNAL(choose_right()), this, SLOT(dial_right()));
}

void Process::disconnectToDial(){

    //disconnect dial signals/slots
    disconnect(this, SIGNAL(showDial(bool)), 0, 0);
    disconnect(this, SIGNAL(showDial(double, double)), 0, 0);
    disconnect(this, SIGNAL(updateDial(double, double)), 0, 0);
    disconnect(this, SIGNAL(doneDial()), 0, 0);
    disconnect(display.widgetDial(), SIGNAL(choose_up()), 0, 0);
    disconnect(display.widgetDial(), SIGNAL(choose_down()), 0, 0);
    disconnect(display.widgetDial(), SIGNAL(choose_left()), 0, 0);
    disconnect(display.widgetDial(), SIGNAL(choose_right()), 0, 0);
}

void Process::disconnectAll(){

    //disconnect all signals/slots in different foreground

    //gesture signals/slots
    disconnect(this, SIGNAL(pinch(double, double, bool)), 0, 0);
    disconnect(this, SIGNAL(grab(int)), 0, 0);
    disconnect(this, SIGNAL(circle(int)), 0, 0);
    disconnect(this, SIGNAL(circle_anti(int)), 0, 0);

    //choose signals/slots
    disconnect(display.widgetDial(), SIGNAL(choose_up(double, double)), operation, 0);
    disconnect(this, SIGNAL(choose_up(QString)), operation, 0);
    disconnect(display.widgetDial(), SIGNAL(choose_down(double, double)), operation, 0);
    disconnect(this, SIGNAL(choose_down(QString)), operation, 0);
    disconnect(display.widgetDial(), SIGNAL(choose_left(double, double)), operation, 0);
    disconnect(this, SIGNAL(choose_left(QString)), operation, 0);
    disconnect(display.widgetDial(), SIGNAL(choose_right(double, double)), operation, 0);
    disconnect(this, SIGNAL(choose_right(QString)), operation, 0);

    //disconnect dial signals/slots
    disconnectToDial();

    logger.log("INFO", "Disconnected All signals and slots between gesture/userevent an operation.");
}

QString Process::getForegroundWindow(){

    //get foreground process name and window name
    Foreground foreground_window;
    QString foreground_window_name = QString::fromStdString(foreground_window.getForegroundWindowName());
    QString foreground_window_process_name = QString::fromStdString(foreground_window.getForegroundProcessName());

    //browser only support Internet Explorer, Firefox, Chrome and Opera
    if(foreground_window_process_name.contains("firefox.exe",Qt::CaseSensitive) ||
    foreground_window_process_name.contains("chrome.exe",Qt::CaseSensitive) ||
    foreground_window_process_name.contains("iexplore.exe",Qt::CaseSensitive) ||
    foreground_window_process_name.contains("opera.exe",Qt::CaseSensitive)
    ){
        return "browser";
    }

    //the desktop and explorer has the same process, so use window name to set the different
    if(foreground_window_process_name.contains("explorer.exe",Qt::CaseSensitive)){
        if(foreground_window_name.contains("Program Manager",Qt::CaseSensitive)){
            return "desktop";
        }else{
            return "explorer";
        }
    }

    //other window
    return "other";

}

void Process::dial_up(){

    //load the dial config
    QVariantMap config_operation = config_operation[getForegroundWindow()].toMap();
    QVariantMap config_operation_dial = config_operation["dial"].toMap();
    //emit choose_up(QString) while dial choose up
    emit choose_up(config_operation_dial["up"].toString());
    logger.log("INFO", "Dial choose to UP.");
}

void Process::dial_down(){

    //load the dial config
    QVariantMap config_operation = config_operation[getForegroundWindow()].toMap();
    QVariantMap config_operation_dial = config_operation["dial"].toMap();
    //emit choose_down(QString) while dial choose down
    emit choose_down(config_operation_dial["down"].toString());
    logger.log("INFO", "Dial choose to DOWN.");
}

void Process::dial_left(){

    //load the dial config
    QVariantMap config_operation = config_operation[getForegroundWindow()].toMap();
    QVariantMap config_operation_dial = config_operation["dial"].toMap();
    //emit choose_left(QString) while dial choose left
    emit choose_left(config_operation_dial["left"].toString());

    logger.log("INFO", "Dial choose to LEFT.");
}
void Process::dial_right(){

    //load the dial config
    QVariantMap config_operation = config_operation[getForegroundWindow()].toMap();
    QVariantMap config_operation_dial = config_operation["dial"].toMap();
    //emit choose_right(QString) while dial choose right
    emit choose_right(config_operation_dial["right"].toString());
    logger.log("INFO", "Dial choose to RIGHT.");
}

void Process::logKeyInfo(){

    //log key information
    logger.log("INFO", "===================== KEY INFORMATION =====================");
    logger.log("INFO", "Controller Status: " + QString::number(motion.getControllerStatus()) + ", Service Status: " + QString::number(motion.getServiceStatus()) + ", Process Status: " + QString::number(motion.getProcessStatus()));
    logger.log("INFO", "Position: x: " + QString::number(motion.getNormalizedX()) + ", y: " + QString::number(motion.getNormalizedY()));
    logger.log("INFO", "Hands Cout: " + motion.getHandCount());
    logger.log("INFO", "===========================================================");
}
