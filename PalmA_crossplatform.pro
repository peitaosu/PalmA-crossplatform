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
    status_widget.cpp \
    display.cpp \
    test.cpp

HEADERS += status_widget.h \
    display.h \
    test.h \
    include/Leap.h \
    include/LeapMath.h \
    error_code.h

FORMS   += status_widget.ui

win32{
    SOURCES += operation_win.cpp \
    event_win.cpp
    HEADERS += operation_win.h \
    event_win.h
}

unix{
    SOURCES += operation_unix.cpp \
    event_unix.cpp
    HEADERS += operation_unix.h \
    event_unix.h
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

