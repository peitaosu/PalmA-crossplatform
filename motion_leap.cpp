#include "motion_leap.h"
#include "motion_code.h"
#include "error_code.h"

MotionLeap::MotionLeap(){
    left_handedness = false;
    controller.enableGesture(Leap::Gesture::TYPE_CIRCLE);
    controller.enableGesture(Leap::Gesture::TYPE_KEY_TAP);
    controller.enableGesture(Leap::Gesture::TYPE_SCREEN_TAP);
    controller.enableGesture(Leap::Gesture::TYPE_SWIPE);
}

int MotionLeap::getControllerType(){
    return LEAP_MOTION;
}

double MotionLeap::getNormalizedX(){
    i_box = controller.frame().interactionBox();
    hand = controller.frame().hands().rightmost();
    return i_box.normalizePoint(hand.stabilizedPalmPosition()).x;
}

double MotionLeap::getNormalizedY(){
    i_box = controller.frame().interactionBox();
    hand = controller.frame().hands().rightmost();
    return 1 - i_box.normalizePoint(hand.stabilizedPalmPosition()).y;
}

double MotionLeap::getNormalizedZ(){
    i_box = controller.frame().interactionBox();
    hand = controller.frame().hands().rightmost();
    return i_box.normalizePoint(hand.stabilizedPalmPosition()).z;
}

int MotionLeap::getGestureSwipe()
{
    Leap::Gesture gesture = controller.frame().gestures()[0];
    if(gesture.type() == Leap::Gesture::TYPE_SWIPE) {
        return 1;
    }else{
        return 0;
    }
}

int MotionLeap::getGestureCircle(bool clockwise)
{
    if(clockwise){
        Leap::Gesture gesture = controller.frame().gestures()[0];
        if(gesture.type() == Leap::Gesture::TYPE_CIRCLE) {
            Leap::CircleGesture circle = gesture;
            if (circle.pointable().direction().angleTo(circle.normal()) <= 3.1415926/2) {
                return 1;
            } else {
                return 0;
            }
        }else{
            return 0;
        }
    }else{
        Leap::Gesture gesture = controller.frame().gestures()[0];
        if(gesture.type() == Leap::Gesture::TYPE_CIRCLE) {
            Leap::CircleGesture circle = gesture;
            if (circle.pointable().direction().angleTo(circle.normal()) <= 3.1415926/2) {
                return 0;
            } else {
                return 1;
            }
        }else{
            return 0;
        }
    }

}

int MotionLeap::getGestureScreenTap()
{
    Leap::Gesture gesture = controller.frame().gestures()[0];
    if(gesture.type() == Leap::Gesture::TYPE_SCREEN_TAP) {
        return 1;
    }else{
        return 0;
    }
}

int MotionLeap::getGestureKeyTap()
{
    Leap::Gesture gesture = controller.frame().gestures()[0];
    if(gesture.type() == Leap::Gesture::TYPE_KEY_TAP) {
        return 1;
    }else{
        return 0;
    }
}

int MotionLeap::getGestureGrab(int hand_count)
{
    double threshold = 0.95;
    if(hand_count == 1){
        if(hand.grabStrength() >= threshold){
            return 1;
        }else{
            return 0;
        }
    }else if(hand_count == 2){
        Leap::Hand hand_1 = controller.frame().hands()[0];
        Leap::Hand hand_2 = controller.frame().hands()[1];
        if(hand_1.grabStrength() >= threshold && hand_2.grabStrength() >= threshold ){
            return 1;
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}

int MotionLeap::getGesturePinch(int hand_count)
{
    double threshold = 0.95;
    if(hand_count == 1){
        if(hand.pinchStrength() >= threshold){
            return 1;
        }else{
            return 0;
        }
    }else if(hand_count == 2){
        Leap::Hand hand_1 = controller.frame().hands()[0];
        Leap::Hand hand_2 = controller.frame().hands()[1];
        if(hand_1.pinchStrength() >= threshold && hand_2.pinchStrength() >= threshold ){
            return 1;
        }else{
            return 0;
        }
    }
    return 0;
}

int MotionLeap::getGestureStatus(int gesture_type, int count){
    switch(gesture_type){
        case SWIPE:
            if(count != 1){
                return INPUT_ERROR;
            }
            return getGestureSwipe();
        case CIRCLE:
            if(count > 1){
                return INPUT_ERROR;
            }
            return getGestureCircle(true);
        case CIRANTI:
            if(count > 1){
                return INPUT_ERROR;
            }
            return getGestureCircle(false);
        case SCREENTAP:
            if(count > 1){
                return INPUT_ERROR;
            }
            return getGestureScreenTap();
        case KEYTAP:
            if(count > 1){
                return INPUT_ERROR;
            }
            return getGestureKeyTap();
        case GRAB:
            if(count > 2 || count < 1){
                return INPUT_ERROR;
            }
            return getGestureGrab(count);
        case PINCH:
            if(count > 2 || count < 1){
                return INPUT_ERROR;
            }
            return getGesturePinch(count);
        default:
            return INPUT_ERROR;
    }
    return 0;
}

int MotionLeap::getHandCount()
{
    return controller.frame().hands().count();
}

void MotionLeap::setHandedness(bool is_left)
{
    left_handedness = is_left;
}

bool MotionLeap::getControllerStatus()
{
    return controller.isConnected();
}

bool MotionLeap::getServiceStatus()
{
    return controller.isServiceConnected();
}

bool MotionLeap::getProcessStatus()
{
    return controller.frame().isValid();
}
