#ifndef MOTION_H
#define MOTION_H
#include "motion_leap.h"

class Motion
{
    public:
        Motion();
        
        bool setController(int controller_type);
        int getControllerCurrent();
        int* getControllerSupported();
        bool getControllerStatus();
        bool getServiceStatus();
        bool getProcessStatus();
        
        double getNormalizedX();
        double getNormalizedY();
        double getNormalizedZ();
        
        int getGestureStatus(int gesture_type, int count = 1);

        void setHandedness(bool is_left = true);

        bool update();
        
    private:
        int controller_type_current;
        int* controller_type_supported;
        
        double x_normalized;
        double y_normalized;
        double z_normalized;
        
        bool left_handedness;

        MotionLeap motion_leap;
};

#endif // MOTION_H
