#ifndef MOTION_LEAP_H
#define MOTION_LEAP_H
#include "include/Leap.h"

class MotionLeap
{
    public:
        MotionLeap();

        int getControllerType();
        
        double getNormalizedX();
        double getNormalizedY();
        double getNormalizedZ();
        
        int getGestureStatus(int gesture_type, int count = 1);
        
    private:
        Leap::Controller controller;
        Leap::InteractionBox i_box;
        
        double x_normalized;
        double y_normalized;
        double z_normalized;
        
        
};

#endif // MOTION_LEAP_H