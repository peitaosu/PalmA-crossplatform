//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2019 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


#ifndef PALMA_MOTION_LEAP_H
#define PALMA_MOTION_LEAP_H
#include "include/Leap.h"

class MotionLeap
{
    public:
        MotionLeap();

        //normal point
        double getNormalizedX();
        double getNormalizedY();
        double getNormalizedZ();
        
        //hand
        int getHandCount();
        void setHandedness(bool is_left = true);

        //gesture
        int getGestureSwipe();
        int getGestureCircle(bool clockwise = true);
        int getGestureScreenTap();
        int getGestureKeyTap();
        int getGestureGrab(int hand_count = 1);
        int getGesturePinch(int hand_count = 1);
        int getGestureStatus(int gesture_type, int count = 1);
        
        //controller
        int getControllerType();

        //status
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

#endif // PALMA_MOTION_LEAP_H
