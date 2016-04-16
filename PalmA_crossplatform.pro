#----------------------------------------------#
#                                              #
# Copyright @ 2016 Tony Su All Rights Reserved #
#                                              #
#----------------------------------------------#

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PalmA_crossplatform
TEMPLATE = app


SOURCES += main.cpp \
    widget_status.cpp \
    display.cpp \
    operation.cpp \
    test.cpp \
    widget_cursor.cpp \
    motion.cpp \
    motion_leap.cpp \
    widget_gesture.cpp

HEADERS += widget_status.h \
    display.h \
    test.h \
    include/Leap.h \
    include/LeapMath.h \
    error_code.h \
    operation.h \
    widget_cursor.h \
    motion.h \
    motion_leap.h \
    motion_code.h \
    widget_gesture.h

FORMS   += widget_status.ui \
    widget_cursor.ui \
    widget_gesture.ui

win32{
    SOURCES += event_win.cpp
    HEADERS += event_win.h
}

unix{
    QT += x11extras
    SOURCES += event_unix.cpp
    HEADERS += event_unix.h
    LIBS += -lX11 -lXtst -lXext
}



LIBS += -L$$PWD/lib/x64/ -lLeap \
    -L$$PWD/lib/x86/ -lLeap

INCLUDEPATH += $$PWD/lib/x64 \
    $$PWD/lib/x86
DEPENDPATH += $$PWD/lib/x64 \
    $$PWD/lib/x86

DISTFILES += \
    include/Leap.i

RESOURCES += \
    json.qrc

