#include "motion.h"
#include "motion_code.h"
#include "error_code.h"

Motion::Motion(){

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

bool setController(int controller_type){
    controller_type_current = controller_type;
    if(controller_type_current == controller_type){
        return true;
    }else{
        return false;
    }
}

int getControllerCurrent(){
    return controller_type_current;
}

int* getControllerSupported(){
    return controller_type_supported;
}

int getGestureStatus(int gesture_type, int count){
    if(controller_type_current == LEAP_MOTION){
        switch(gesture_type){
            case SWIPE:
                //TODO: add function
                break;
            case CIRCLE:
                //TODO: add function
                break;
            case SCREENTAP:
                //TODO: add function
                break;
            case KEYTAP:
                //TODO: add function
                break;
            case GRAB:
                //TODO: add function
                break;
            case PINCH:
                //TODO: add function
                break;
            default:
                break;
        }
    }else if(controller_type_current == KINECT){
        //TODO: add Kinect support
    }else{
        return INPUT_ERROR;
    }
}