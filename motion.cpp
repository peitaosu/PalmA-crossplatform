//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2017 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


#include "motion.h"
#include "motion_code.h"
#include "error_code.h"
#include <iostream>

Motion::Motion(){
    //set handedness, default right
    left_handedness = false;
}

/*
 * Motion: Get Normalized X
 * Input: NONE
 * Return: Normalized position x: [0,1]
 */
double Motion::getNormalizedX(){
    return x_normalized;
}

/*
 * Motion: Get Normalized Y
 * Input: NONE
 * Return: Normalized position y: [0,1]
 */
double Motion::getNormalizedY(){
    return y_normalized;
}

/*
 * Motion: Get Normalized Z
 * Input: NONE
 * Return: Normalized position z: [0,1]
 */
double Motion::getNormalizedZ(){
    return z_normalized;
}

/*
 * Motion: Set Controller
 * Input: int controller type, defined in motion_code.h
 * Return: setup succeed or not
 */
bool Motion::setController(int controller_type){
    controller_type_current = controller_type;
    if(controller_type_current == controller_type){
        return true;
    }else{
        return false;
    }
}

/*
 * Motion: Get Current Controller
 * Input: NONE
 * Return: int controller type, defined in motion_code.h
 */
int Motion::getControllerCurrent(){
    return controller_type_current;
}

/*
 * Motion: Get Supported Controller List
 * Input: NONE
 * Return: int controller type list, defined in motion_code.h
 */
int* Motion::getControllerSupported(){
    return controller_type_supported;
}

/*
 * Motion: Get Controller Status
 * Input: NONE
 * Return: bool Connected or not
 */
bool Motion::getControllerStatus(){
    if(controller_type_current == LEAP_MOTION){

        //for LEAP MOTION
        return motion_leap.getControllerStatus();
    }else if(controller_type_current == KINECT){

        //for KINECT
        return false;
    }
    return false;
}

/*
 * Motion: Get Service Status
 * Input: NONE
 * Return: bool Connected or not
 */
bool Motion::getServiceStatus(){
    if(controller_type_current == LEAP_MOTION){

        //for LEAP MOTION
        return motion_leap.getServiceStatus();
    }else if(controller_type_current == KINECT){

        //for KINECT
        return false;
    }
    return false;
}

/*
 * Motion: Get Process Status
 * Input: NONE
 * Return: bool Connected or not
 */
bool Motion::getProcessStatus(){
    if(controller_type_current == LEAP_MOTION){

        //for LEAP MOTION
        return motion_leap.getProcessStatus();
    }else if(controller_type_current == KINECT){

        //for KINECT
        return false;
    }
    return false;
}

/*
 * Motion: Get Gestures Status
 * Input: int Gesture Type, int Hands Count, defined in motion_code.h
 * Return: 1 or 0, means yes or no
 */
int Motion::getGestureStatus(int gesture_type, int count){
    if(controller_type_current == LEAP_MOTION){

        //for LEAP MOTION
        MotionLeap motion_leap;

        //gesture_status is a 10*2 array, first index is gesture type defined in motion_code.h, second index is hand count
        gesture_status[gesture_type*count][1] = gesture_status[gesture_type*count][0];
        gesture_status[gesture_type*count][0] = motion_leap.getGestureStatus(gesture_type, count);

        return gesture_status[gesture_type*count][0];
    }else if(controller_type_current == KINECT){

        //for KINECT
        return 0;
    }else{
        return INPUT_ERROR;
    }
}

/*
 * Motion: Get Gestures Event
 * Input: int Gesture Type, int Hands Count, defined in motion_code.h
 * Return: NONE, START, KEEP and STOP, defined in motion_code.h
 */
int Motion::getGestureEvent(int gesture_type, int count){

    if(controller_type_current == LEAP_MOTION){

        //for LEAP MOTION
        gesture_status[gesture_type*count][1] = gesture_status[gesture_type][0];
        gesture_status[gesture_type*count][0] = motion_leap.getGestureStatus(gesture_type, count);

        //if 0 previous and 0 now
        if(gesture_status[gesture_type*count][1] == 0 &&
        gesture_status[gesture_type*count][0] == 0){
            return NONE;
        }
        //if 0 previous and 1 now
        if(gesture_status[gesture_type*count][1] == 0 &&
        gesture_status[gesture_type*count][0] == 1){
            return START;
        }
        //if 1 previous and 1 now
        if(gesture_status[gesture_type*count][1] == 1 &&
        gesture_status[gesture_type*count][0] == 1){
            return KEEP;
        }
        //if 1 previous and 0 now
        if(gesture_status[gesture_type*count][1] == 1 &&
        gesture_status[gesture_type*count][0] == 0){
            return STOP;
        }

        return 0;
    }else if(controller_type_current == KINECT){

        //for KINECT
        return 0;
    }else{
        return INPUT_ERROR;
    }
}

/*
 * Motion: Get Hands Count
 * Input: NONE
 * Return: int Hands Count
 */
int Motion::getHandCount(){
    if(controller_type_current == LEAP_MOTION){

        //for LEAP MOTION
        return motion_leap.getHandCount();
    }else if(controller_type_current == KINECT){

        //for KINECT
        return 0;
    }
    return 0;
}

/*
 * Motion: Set Handedness
 * Input: bool, isleft or not
 * Return: NONE
 */
void Motion::setHandedness(bool is_left)
{
    left_handedness = is_left;
    if(controller_type_current == LEAP_MOTION){

        //for LEAP MOTION
        motion_leap.setHandedness(is_left);
    }else if(controller_type_current == KINECT){

        //for KINECT
    }
}

/*
 * Motion: Update
 * Input: NONE
 * Return: if set succeed, return true
 */
bool Motion::update()
{
    if(controller_type_current == LEAP_MOTION){

        //for LEAP MOTION
        x_normalized = motion_leap.getNormalizedX();
        y_normalized = motion_leap.getNormalizedY();
        z_normalized = motion_leap.getNormalizedZ();
        return true;
    }else if(controller_type_current == KINECT){

        //for KINECT
        return true;
    }
    return false;
}
