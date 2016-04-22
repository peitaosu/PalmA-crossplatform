#include "test.h"
#include "display.h"
#include "include/Leap.h"
#include "operation.h"
#include "event.h"
#include "foreground.h"
#include "logger.h"

#include <iostream>
Test::Test(){

}

void Test::testAllClasses(){
    Display display;
    Leap::Controller controller;
    Leap::Listener listener;
    controller.addListener(listener);
    Foreground foreground;
    Logger logger;
}
