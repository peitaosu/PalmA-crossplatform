#ifndef MOTION_H
#define MOTION_H
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

#endif // MOTION_H
