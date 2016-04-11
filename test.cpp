#include "test.h"
#include "display.h"
#include "operation_win.h"
#include "include/Leap.h"

Test::Test(){

}

void Test::testAllClasses(){
    Display display;
    Operation operation;
    Leap::Controller controller;
    Leap::Listener listener;
    controller.addListener(listener);
    operation.mouseMove(0.8, 0.8);
    operation.mousePress();
    operation.mouseRelease();
}
