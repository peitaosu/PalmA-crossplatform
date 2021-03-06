//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2019 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


#include "motion_leap.h"
#include "motion_code.h"
#include "error_code.h"

MotionLeap::MotionLeap(){
    left_handedness = false;
    //enable built-in gestures
    controller.enableGesture(Leap::Gesture::TYPE_CIRCLE);
    controller.enableGesture(Leap::Gesture::TYPE_KEY_TAP);
    controller.enableGesture(Leap::Gesture::TYPE_SCREEN_TAP);
    controller.enableGesture(Leap::Gesture::TYPE_SWIPE);
}

/*
 * Motion: Get Controller Type
 * Input: NONE
 * Return: int LEAP_MOTION, defined in motion_code.h
 */
int MotionLeap::getControllerType(){
    return LEAP_MOTION;
}

/*
 * Motion: Get Normalized X
 * Input: NONE
 * Return: Normalized position x: [0,1]
 */
double MotionLeap::getNormalizedX(){
    i_box = controller.frame().interactionBox();
    //set hand to leftmost or rightmost according to handedness
    if(left_handedness){
        hand = controller.frame().hands().leftmost();
    }else{
        hand = controller.frame().hands().rightmost();
    }
    //return stabilized position
    return i_box.normalizePoint(hand.stabilizedPalmPosition()).x;
}

/*
 * Motion: Get Normalized Y
 * Input: NONE
 * Return: Normalized position y: [0,1]
 */
double MotionLeap::getNormalizedY(){
    i_box = controller.frame().interactionBox();
    //set hand to leftmost or rightmost according to handedness
    if(left_handedness){
        hand = controller.frame().hands().leftmost();
    }else{
        hand = controller.frame().hands().rightmost();
    }
    //return stabilized position
    return 1 - i_box.normalizePoint(hand.stabilizedPalmPosition()).y;
}

/*
 * Motion: Get Normalized Z
 * Input: NONE
 * Return: Normalized position z: [0,1]
 */
double MotionLeap::getNormalizedZ(){
    i_box = controller.frame().interactionBox();
    //set hand to leftmost or rightmost according to handedness
    if(left_handedness){
        hand = controller.frame().hands().leftmost();
    }else{
        hand = controller.frame().hands().rightmost();
    }
    //return stabilized position
    return i_box.normalizePoint(hand.stabilizedPalmPosition()).z;
}

/*
 * Motion: Get Gesture Swipe Status
 * Input: NONE
 * Return: 1 or 0, means yes or no
 */
int MotionLeap::getGestureSwipe(){
    //get current gesture
    Leap::Gesture gesture = controller.frame().gestures()[0];
    //swipe
    if(gesture.type() == Leap::Gesture::TYPE_SWIPE) {
        return 1;
    }else{
        return 0;
    }
}

/*
 * Motion: Get Gesture Circle Status
 * Input: NONE
 * Return: 1 or 0, means yes or no
 */
int MotionLeap::getGestureCircle(bool clockwise){
    //if clockwise
    if(clockwise){
        //get current gesture
        Leap::Gesture gesture = controller.frame().gestures()[0];
        //circle (clockwise)
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
        //get current gesture
        Leap::Gesture gesture = controller.frame().gestures()[0];
        //circle (anti-clockwise )
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

/*
 * Motion: Get Gesture Screen Tap Status
 * Input: NONE
 * Return: 1 or 0, means yes or no
 */
int MotionLeap::getGestureScreenTap(){
    //get current gesture
    Leap::Gesture gesture = controller.frame().gestures()[0];
    //screen tap
    if(gesture.type() == Leap::Gesture::TYPE_SCREEN_TAP) {
        return 1;
    }else{
        return 0;
    }
}

/*
 * Motion: Get Gesture Key Tap Status
 * Return: 1 or 0, means yes or no
 */
int MotionLeap::getGestureKeyTap(){
    //get current gesture
    Leap::Gesture gesture = controller.frame().gestures()[0];
    //key tap
    if(gesture.type() == Leap::Gesture::TYPE_KEY_TAP) {
        return 1;
    }else{
        return 0;
    }
}

/*
 * Motion: Get Gesture Grab Status
 * Input: int Hands Count
 * Return: 1 or 0, means yes or no
 */
int MotionLeap::getGestureGrab(int hand_count){
    //threshold for grab strength
    double threshold = 0.95;
    if(hand_count == 1){
        //one hand for handedness
        if(left_handedness){
            hand = controller.frame().hands().leftmost();
        }else{
            hand = controller.frame().hands().rightmost();
        }
        if(hand.grabStrength() >= threshold){
            return 1;
        }else{
            return 0;
        }
    }else if(hand_count == 2){
        //two hands in handlist
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

/*
 * Motion: Get Gesture Pinch Status
 * Input: int Hands Count
 * Return: 1 or 0, means yes or no
 */
int MotionLeap::getGesturePinch(int hand_count){
    //threshold for pinch strength
    double threshold = 0.95;
    if(hand_count == 1){
        //one hand for handedness
        if(left_handedness){
            hand = controller.frame().hands().leftmost();
        }else{
            hand = controller.frame().hands().rightmost();
        }
        if(hand.pinchStrength() >= threshold){
            return 1;
        }else{
            return 0;
        }
    }else if(hand_count == 2){
        //two hands in handlist
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

/*
 * Motion: Get Gestures Status
 * Input: int Gesture Type, int Hands Count, defined in motion_code.h
 * Return: 1 or 0, means yes or no
 */
int MotionLeap::getGestureStatus(int gesture_type, int count){
    //return gesture status according to gesture type
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

/*
 * Motion: Get Hands Count
 * Input: NONE
 * Return: int Hands Count
 */
int MotionLeap::getHandCount(){
    //hands count
    return controller.frame().hands().count();
}

/*
 * Motion: Get Handedness
 * Input: bool, isleft or not
 * Return: NONE
 */
void MotionLeap::setHandedness(bool is_left){
    left_handedness = is_left;
    if(left_handedness == true){
        //if handedness is left
        hand = controller.frame().hands().leftmost();
    }else{
        //else handedness is right
        hand = controller.frame().hands().rightmost();
    }
}

/*
 * Motion: Get Controller Status
 * Input: NONE
 * Return: bool Connected or not
 */
bool MotionLeap::getControllerStatus(){
    //controller status
    return controller.isConnected();
}

/*
 * Motion: Get Service Status
 * Input: NONE
 * Return: bool Connected or not
 */
bool MotionLeap::getServiceStatus(){
    //service status
    return controller.isServiceConnected();
}

/*
 * Motion: Get Process Status
 * Input: NONE
 * Return: bool Connected or not
 */
bool MotionLeap::getProcessStatus(){
    //frame status
    return controller.frame().isValid();
}
