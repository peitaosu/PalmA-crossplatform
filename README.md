# PalmA-crossplatform

**PalmA** is a crossplatform somatosensory application to assist computer user interface control.

The **1st** version was contributed by Peitao Su, Aiwei Yu, Erbo Yu and Xiaoli Sheng from 2014 to 2015 in Shanghai Ocean University.

**PalmA for Crossplatform** version was re-written by Peitao Su in April 2016.

**PalmA for IoT** version was written by Aiwei Yu in April 2016.

License
=======
Apache License Version 2.0

HOW-TOs
=======

1. Get the source code:
```
> git clone https://github.com/peitaosu/PalmA-crossplatform
```

2. Setup your development environment:

    Download and Install:
    * **Qt Opensource**
    * **Visual Studio**(for Windows)
    * **g++**(for Linux)
    
    Make sure the Qt was compiled with the same version. For example, in my environment, it should be `qt-opensource-windows-x86-msvc2015_64-5.6.0` and `Visual Studio Community(2015)`.

3. Setup devices environment:

    * Get Leap Motion SDK from: https://developer.leapmotion.com/v2, install Leap Motion Service, extract SDK.
    * Get Kinect SDK from: https://developer.microsoft.com/en-us/windows/kinect or https://openkinect.org/
  
Quick Start
===========
1. Import Project:

    Open Qt-Creater, import PalmA_crossplatform.pro, setup your build configurations (Compiler, Debugger and Output folder).

2. Copy header and libraries:

    From Leap Motion build, copy /include and /lib folders in Leap Motion SDK to project folder.

3. Make & Build

    Run qmake & Build Project.

4. Run

    Plug-in device, launch the application.

Known Issue
===========
1. The widget header include path will cause a build error. (Ubuntu 14.04, with Qt 5.6.0, g++ 4.8.4)

2. QTextStream will cause a build error. (Ubuntu 14.04, with Qt 5.6.0, g++ 4.8.4)

3. Display will case a previous declaration error. (Ubuntu 14.04, with Qt 5.6.0, g++ 4.8.4)
