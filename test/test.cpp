#include "test.h"
#include "display.h"
#include "operation.h"
#include "event.h"
#include "foreground.h"
#include "logger.h"
#include "process.h"
#include "motion.h"
Test::Test(){

}

void Test::testAllClasses(){
    Process process;
    Motion motion;
    Operation operation;
    Display display;
    Foreground foreground;
    Logger logger;

}

void Test::testGoThrough()
{
    Process process;
    process.init();
    process.start();
    Motion motion;
    motion.getControllerCurrent();
    Operation operation;
    operation.showDesktop();
    Display display;
    display.showDial("desktop", 0.5, 0.5);
    Foreground foreground;
    Logger logger;
    logger.log("INFO", "TEST GO THROUGN");
}
