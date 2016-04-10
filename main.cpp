#include <QApplication>
#include "display.h"

int main(int argc, char *argv[])
{
    QApplication palm_crossplatform(argc, argv);
    Display D;
    return palm_crossplatform.exec();
}
