#include "motion.h"
#include "motion_code.h"
#include "error_code.h"

Motion::Motion(){
    left_handedness = false;
}

double Motion::getNormalizedX(){
    return x_normalized;
}

double Motion::getNormalizedY(){
    return y_normalized;
}

double Motion::getNormalizedZ(){
    return z_normalized;
}

bool Motion::setController(int controller_type){
    controller_type_current = controller_type;
    if(controller_type_current == controller_type){
        return true;
    }else{
        return false;
    }
}

int Motion::getControllerCurrent(){
    return controller_type_current;
}

int* Motion::getControllerSupported(){
    return controller_type_supported;
}

int Motion::getGestureStatus(int gesture_type, int count){
    if(controller_type_current == LEAP_MOTION){
        MotionLeap motion_leap;
        return motion_leap.getGestureStatus(gesture_type, count);
    }else if(controller_type_current == KINECT){
        //TODO: add Kinect support
        return 0;
    }else{
        return INPUT_ERROR;
    }
}

void Motion::setHandedness(bool is_left)
{
    left_handedness = is_left;
    motion_leap.setHandedness(is_left);
}

bool Motion::update()
{
    if(controller_type_current == LEAP_MOTION){
        x_normalized = motion_leap.getNormalizedX();
        y_normalized = motion_leap.getNormalizedY();
        z_normalized = motion_leap.getNormalizedZ();
        return true;
    }else if(controller_type_current == KINECT){
        //TODO: add Kinect support
        return true;
    }
    return false;
}
