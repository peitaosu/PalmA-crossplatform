//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2017 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


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
