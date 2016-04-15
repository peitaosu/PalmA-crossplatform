#include "motion_leap.h"
#include "motion_code.h"
#include "error_code.h"

MotionLeap::MotionLeap(){
    i_box = controller.frame().interactionBox();
}

int getControllerType(){
    return LEAP_MOTION;
}

double MotionLeap::getNormalizedX(){
    return i_box.normalizePoint(controller.frame().hands()[0].stabilizedPalmPosition()).x;
}

double MotionLeap::getNormalizedY(){
    return i_box.normalizePoint(controller.frame().hands()[0].stabilizedPalmPosition()).y;
}

double MotionLeap::getNormalizedZ(){
    return i_box.normalizePoint(controller.frame().hands()[0].stabilizedPalmPosition()).z;
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
        Leap::Hand hand = controller.frame().hands()[0];
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
    double threshold = 0.80;
    if(hand_count == 1){
        Leap::Hand hand = controller.frame().hands()[0];
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
                //TODO: LOG ERROR
                break;
            }
            return getGestureSwipe();
        case CIRCLE:
            if(count > 1){
                //TODO: LOG ERROR
                break;
            }
            return getGestureCircle(true);
        case CIRANTI:
            if(count > 1){
                //TODO: LOG ERROR
                break;
            }
            return getGestureCircle(false);
        case SCREENTAP:
            if(count > 1){
                //TODO: LOG ERROR
                break;
            }
            return getGestureScreenTap();
        case KEYTAP:
            if(count > 1){
                //TODO: LOG ERROR
                break;
            }
            return getGestureKeyTap();
        case GRAB:
            return getGestureGrab(count);
        case PINCH:
            return getGesturePinch(count);
        default:
            //TODO: LOG ERROR
            break;
    }
    return 0;
}
