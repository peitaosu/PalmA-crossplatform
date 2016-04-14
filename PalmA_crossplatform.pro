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
    operation.cpp \
    test.cpp \
    cursor_widget.cpp

HEADERS += status_widget.h \
    display.h \
    test.h \
    include/Leap.h \
    include/LeapMath.h \
    error_code.h \
    operation.h \
    cursor_widget.h

FORMS   += status_widget.ui \
    cursor_widget.ui

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

