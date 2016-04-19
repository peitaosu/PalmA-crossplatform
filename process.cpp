#include "process.h"
#include "motion_code.h"

Process::Process(QObject *parent) : QObject(parent)
{
    loop_timer = new QTimer(this);


}

void Process::init(int argc, char* argv[])
{
    for(int arg = 1; arg < argc; arg ++){
        if(argv[arg] == "-d"){
            display.showStatus();
        }else if(argv[arg] == "-s"){
            display.hideGesture();
        }else if(argv[arg] == "-h"){
            display.showPalm();
        }
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
    motion.setController(LEAP_MOTION);
    motion.update();
    //motion.setHandedness();
    display.updateCursor(motion.getNormalizedX(), motion.getNormalizedY());
    display.updateStatus(motion.getControllerStatus()*4 + motion.getServiceStatus()*2 + motion.getProcessStatus());
    
    if(motion.hand_count == 2){
        //isAllGrab?
        int grab_status_with_hands = motion.getGestureEvent(GRAB, 2);
        switch (grab_status_with_hands){
            case NONE:
                emit grab(NONE);
                break;
            case START:
                emit grab(START);
                break;
            case KEEP:
                emit grab(KEEP);
                break;
            case STOP:
                emit grab(STOP);
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
                break;
            case KEEP:
                emit grab(motion.getNormalizedX(), motion.getNormalizedY());
                break;
            case STOP:
                emit grab(motion.getNormalizedX(), motion.getNormalizedY(), 0);
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
                break;
            case KEEP:
                emit pinch(KEEP);
                break;
            case STOP:
                emit pinch(STOP);
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
                break;
            case KEEP:
                emit pinch(motion.getNormalizedX(), motion.getNormalizedY());
                break;
            case STOP:
                emit pinch(motion.getNormalizedX(), motion.getNormalizedY(), STOP);
                break;
            default:
                break;
        }
    }else if(hand_count == 1){
        //isGrab?
        int grab_status = motion.getGestureEvent(GRAB, 1);
        switch (grab_status){
            case NONE:
                emit grab(NONE);
                break;
            case START:
                emit grab(motion.getNormalizedX(), motion.getNormalizedY(), START);
                break;
            case KEEP:
                emit grab(motion.getNormalizedX(), motion.getNormalizedY());
                break;
            case STOP:
                emit grab(motion.getNormalizedX(), motion.getNormalizedY(), STOP);
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
                break;
            case KEEP:
                emit pinch(motion.getNormalizedX(), motion.getNormalizedY());
                break;
            case STOP:
                emit pinch(motion.getNormalizedX(), motion.getNormalizedY(), STOP);
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
                break;
            case KEEP:
                emit circle(KEEP);
                break;
            case STOP:
                emit circle(STOP);
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
                break;
            case KEEP:
                emit circle_anti(KEEP);
                break;
            case STOP:
                emit circle_anti(STOP);
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
                break;
            case KEEP:
                emit swipe(KEEP);
                break;
            case STOP:
                emit swipe(STOP);
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

void Process::stop()
{
    disconnect(loop_timer,SIGNAL(timeout()),this,SLOT(run()));
}

void Process::restart()
{
    stop();
    start();
}
