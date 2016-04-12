#include "test.h"
#include "display.h"
#include "include/Leap.h"
#include "event_win.h"

Test::Test(){

}

void Test::testAllClasses(){
    Display display;
    Leap::Controller controller;
    Leap::Listener listener;
    controller.addListener(listener);
    Event event;
    event.keyboardMType("LWIN+R");
}
