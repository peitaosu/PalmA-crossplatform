#include "test.h"
#include "display.h"
#include "include/Leap.h"
#include "operation.h"
#include "event_win.h"
#include "event_unix.h"
#include "cursor_widget.h"

Test::Test(){

}

void Test::testAllClasses(){
    Display display;
    Leap::Controller controller;
    Leap::Listener listener;
    controller.addListener(listener);
    //Operation operation;
    //operation.logoff();
    //Event event;
    //event.keyboardMType("LWIN+L");
}
