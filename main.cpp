#include <QApplication>
#include "status_widget.h"

int main(int argc, char *argv[])
{
    QApplication palm_crossplatform(argc, argv);
    StatusWidget S;
    S.show();
    return palm_crossplatform.exec();
}
