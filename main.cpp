#include <QApplication>
#include "test.h"

int main(int argc, char *argv[])
{
    QApplication palm_crossplatform(argc, argv);
    Test test;
    test.testAllClasses();
    return palm_crossplatform.exec();
}
