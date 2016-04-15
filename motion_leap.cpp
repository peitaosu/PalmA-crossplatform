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

int MotionLeap::getGestureStatus(int gesture_type, int count){
    switch(gesture_type){
        case SWIPE:
            if(count > 1){
                //TODO: LOG ERROR
                break;
            }
            //TODO: add function
            break;
        case CIRCLE:
            if(count > 1){
                //TODO: LOG ERROR
                break;
            }
            //TODO: add function
            break;
        case SCREENTAP:
            if(count > 1){
                //TODO: LOG ERROR
                break;
            }
            //TODO: add function
            break;
        case KEYTAP:
            if(count > 1){
                //TODO: LOG ERROR
                break;
            }
            //TODO: add function
            break;
        case GRAB:
            if(count == 1){
                //TODO: add function
                break;
            }else if(count == 2){
                //TODO: add function
                break;
            }else{
                //TODO: LOG ERROR
                break;
            }
            break;
        case PINCH:
            if(count == 1){
                //TODO: add function
                break;
            }else if(count == 2){
                //TODO: add function
                break;
            }else{
                //TODO: LOG ERROR
                break;
            }
            break;
        default:
            //TODO: LOG ERROR
            break;
    }
    return 0;
}
