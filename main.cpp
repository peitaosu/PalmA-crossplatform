#include <QApplication>
#include "process.h"
#include "display.h"

int main(int argc, char *argv[])
{

    QApplication palm_crossplatform(argc, argv);
    Process process;
    process.init(argc, argv);
    process.start();
    process.run();
    return palm_crossplatform.exec();
}
