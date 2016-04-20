#include "process.h"
#include "motion_code.h"
#include <QJsonDocument>
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
    config_gesture = config["gesture"].toVariant().toMap();
    config_display = config["display"].toVariant().toMap();
    config = config["config"].toVariant().toMap();
    

    connect(this, SIGNAL(setGesture(QString)), this, SLOT(showGesture(QString)));

}

void Process::init(int argc, char* argv[])
{
    
    if(config_display["palm"] == "true"){
        display.showPalm();
    }else if(config_display["palm"] == "false"){
        display.hidePalm();
    }
    if(config_display["gesture"] == "true"){
        display.showGesture();
    }else if(config_display["gesture"] == "false"){
        display.hideGesture();
    }
    if(config_display["status"] == "true"){
        display.showStatus();
    }else if(config_display["status"] == "false"){
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
    
    if(config["controller"] == "LEAPMOTION"){
        motion.setController(LEAP_MOTION);
    }
    
    if(config["handedness"] == "left"){
        motion.setHandedness(true);
    }else if(config["handedness"] == "right"){
        motion.setHandedness(false);
    }
        

    //for test
    display.showStatus();
    display.showGesture();
    display.showPalm();
    
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

        if(motion.getHandCount() == 2){
            //isAllGrab?
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
            //isOneGrab?
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
            //isAllPinch?
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
            //isOnePinch?
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
        }else if(motion.getHandCount() == 1){
            //isGrab?
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
            //isPinch?
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
                    //emit setGesture("pinche");
                    emit setGesture("hand");
                    break;
                default:
                    break;
            }
            //isCircle?
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
            //isCircle-Anti?
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
            //isSwipe?
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
            //isScreenTap?
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
            //isKeyTap?
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

void Process::mouse(double x, double y)
{

}

void Process::mouse(double x, double y, int event)
{

}


void Process::showGesture(QString gesture_type)
{
    display.showGesture();
    display.setGestureDisplay(gesture_type);
}

