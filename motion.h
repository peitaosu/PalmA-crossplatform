#ifndef MOTION_H
#define MOTION_H
#include "include/Leap.h"

class Motion
{
    public:
        Motion();
        
        bool setController(int controller_type);
        int getControllerCurrent();
        int* getControllerSupported();
        
        double getNormalizedX();
        double getNormalizedY();
        double getNormalizedZ();
        
        int getGestureStatus(int gesture_type);
        
    private:
        int controller_type_current;
        int* controller_type_supported;
        
        double x_normalized;
        double y_normalized;
        double z_normalized;
        
        //for Leap Motion
        Leap::Controller controller_leap;
        Leap::InteractionBox box_leap;
        
};

#endif // MOTION_H