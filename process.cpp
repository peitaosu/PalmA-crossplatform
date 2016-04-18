#include "process.h"
#include "motion_code.h"

Process::Process(QObject *parent) : QObject(parent)
{
    loop_timer = new QTimer(this);


}

void Process::init(int argc, char* argv[])
{
    for(int arg = 1; arg < argc; arg ++){
        if(argv[arg] == "-d"){
            display.showStatus();
        }else if(argv[arg] == "-s"){
            display.hideGesture();
        }else if(argv[arg] == "-h"){
            display.showPalm();
        }
    }
    //for test
    display.showStatus();
    display.showGesture();
    display.showPalm();
}

void Process::start()
{
    connect(loop_timer,SIGNAL(timeout()),this,SLOT(run()));
    loop_timer->start(20);
}

void Process::run()
{
    motion.setController(LEAP_MOTION);
    motion.update();
    display.updateCursor(motion.getNormalizedX(), motion.getNormalizedY());
    display.updateStatus(motion.getControllerStatus()*4 + motion.getServiceStatus()*2 + motion.getProcessStatus());
    //std::cout<<motion.getNormalizedX()<<std::endl;
    //motion.getNormalizedX()
}

void Process::stop()
{
    disconnect(loop_timer,SIGNAL(timeout()),this,SLOT(run()));
}

void Process::restart()
{
    stop();
    start();
}
