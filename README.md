# PalmA-crossplatform

[![GitHub license](https://img.shields.io/github/license/peitaosu/PalmA-crossplatform.svg)](https://github.com/peitaosu/PalmA-crossplatform/blob/master/LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/peitaosu/PalmA-crossplatform.svg)](https://github.com/peitaosu/PalmA-crossplatform/stargazers)
[![GitHub issues](https://img.shields.io/github/issues/peitaosu/PalmA-crossplatform.svg)](https://github.com/peitaosu/PalmA-crossplatform/issues)

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


Supported Gestures
==================
SWIPE | CIRCLE | CIRCLE_ | SCREENTAP | KEYTAP | GRAB | GRAB_ | PINCH | PINCH_
:----:|:------:|:-------:|:---------:|:------:|:----:|:-----:|:-----:|:-----:
![](/resource/gesture/swipe.png) | ![](/resource/gesture/circle.png) | ![](/resource/gesture/circle_anti.png) | ![](/resource/gesture/screentap.png) | ![](/resource/gesture/keytap.png) | ![](/resource/gesture/grab.png) | ![](/resource/gesture/hand.png) | ![](/resource/gesture/pinch.png) | ![](/resource/gesture/pinche.png)

Supported Features
==================
Desktop | Explorer
:------:|:-------:
Go - File - Setting - Browser | Go - Home - Back - Refresh
![](/resource/dial/dial_desktop.png) | ![](/resource/dial/dial_explorer.png)


Video Introduction
==================
[![PalmA - Open Source Leap Motion Application](https://img.youtube.com/vi/EmmUr-JdGwU/0.jpg)](https://www.youtube.com/watch?v=EmmUr-JdGwU)
