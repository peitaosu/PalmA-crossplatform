#ifndef OPERATION_H
#define OPERATION_H

namespace System {
    class Operation;
}

class Operation
{
    public:
        Operation();

        //mouse operation
        void mouseMove(double x, double y);
        int mousePress(int left_or_right = 1);
        int mouseRelease(int left_or_right = 1);
        int mouseClick(int left_or_right = 1);
        int mouseDClick(int left_or_right = 1);
        void mouseRoll();

        //keyboard operation
        void keyboardPress(char key);
        void keyboardRelease(char key);
        void keyboardType(char key);
        void keyboardMType(char* multi_key);
};

#endif // OPERATION_H
