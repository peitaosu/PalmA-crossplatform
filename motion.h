#ifndef MOTION_H
#define MOTION_H

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
        
        int getGestureStatus(int gesture_type, int count = 1);
        
    private:
        int controller_type_current;
        int* controller_type_supported;
        
        double x_normalized;
        double y_normalized;
        double z_normalized;
        
        
};

#endif // MOTION_H