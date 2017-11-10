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
    display.cpp \
    operation.cpp \
    motion.cpp \
    motion_leap.cpp \
    widget/widget_gesture.cpp \
    widget/widget_dial.cpp \
    widget/widget_palm.cpp \
    widget/widget_cursor.cpp \
    widget/widget_status.cpp \
    logger.cpp \
    test/test.cpp \
    process.cpp

HEADERS += display.h \
    include/Leap.h \
    include/LeapMath.h \
    error_code.h \
    operation.h \
    motion.h \
    motion_leap.h \
    motion_code.h \
    widget/widget_gesture.h \
    widget/widget_dial.h \
    widget/widget_palm.h \
    widget/widget_cursor.h \
    widget/widget_status.h \
    logger.h \
    test/test.h \
    process.h

FORMS   += widget/widget_status.ui \
    widget/widget_cursor.ui \
    widget/widget_gesture.ui \
    widget/widget_dial.ui \
    widget/widget_palm.ui

win32{
    INCLUDEPATH += $$PWD/win/ \
                    $$PWD/widget/
    SOURCES += win/event.cpp \
    win/foreground.cpp
    HEADERS += win/event.h \
    win/foreground.h
}

unix:!macx{
    QT += x11extras
    INCLUDEPATH += $$PWD/linux/
    SOURCES += linux\event.cpp \
    linux\foreground.cpp
    HEADERS += linux\event.h \ \
    linux\foreground.h
    LIBS += -lX11 -lXtst -lXext
}

macx{
    INCLUDEPATH += $$PWD/mac/
    SOURCES += mac\foreground.cpp
    HEADERS += mac\event.h \
    mac\foreground.h
    OBJECTIVE_SOURCES += mac\event.mm
    LIBS += -framework ApplicationServices
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
    json.qrc \
    picture.qrc

