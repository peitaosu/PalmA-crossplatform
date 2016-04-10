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
        void mouseMove(int x, int y);
        void mousePress(int left_or_right);
        void mouseRelease(int left_or_right);
        void mouseClick(int left_or_right);
        void mouseDClick(int left_or_right);
        void mouseRoll();

        //keyboard operation
        void keyboardPress(char key);
        void keyboardRelease(char key);
        void keyboardType(char key);
        void keyboardMType(char* multi_key);
};

#endif // OPERATION_H
