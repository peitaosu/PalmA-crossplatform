//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2017 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


#ifndef PALMA_MOTION_H
#define PALMA_MOTION_H
#include "motion_leap.h"

class Motion
{
    public:
        Motion();
        
        //controller
        bool setController(int controller_type);
        int getControllerCurrent();
        int* getControllerSupported();

        //status
        bool getControllerStatus();
        bool getServiceStatus();
        bool getProcessStatus();
        
        //normalized position
        double getNormalizedX();
        double getNormalizedY();
        double getNormalizedZ();
        
        //gesture
        int getGestureStatus(int gesture_type, int count = 1);
        int getGestureEvent(int gesture_type, int count = 1);

        //hand
        int getHandCount();
        void setHandedness(bool is_left = true);

        //update
        bool update();
        
    private:

        int controller_type_current;
        int* controller_type_supported;

        double x_normalized;
        double y_normalized;
        double z_normalized;
        
        bool left_handedness;

        int gesture_status[10][2];

        MotionLeap motion_leap;
};

#endif // PALMA_MOTION_H
