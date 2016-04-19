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
        
        int getGestureSwipe();
        int getGestureCircle(bool clockwise = true);
        int getGestureScreenTap();
        int getGestureKeyTap();
        int getGestureGrab(int hand_count = 1);
        int getGesturePinch(int hand_count = 1);
        int getGestureStatus(int gesture_type, int count = 1);
        int getHandCount();
        void setHandedness(bool is_left = true);

        bool getControllerStatus();
        bool getServiceStatus();
        bool getProcessStatus();

    private:
        Leap::Controller controller;
        Leap::InteractionBox i_box;
        Leap::Hand hand;
        
        double x_normalized;
        double y_normalized;
        double z_normalized;

        bool left_handedness;
        
        
};

#endif // MOTION_LEAP_H
