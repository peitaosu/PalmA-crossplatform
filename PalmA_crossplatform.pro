#----------------------------------------------#
#                                              #
# Copyright @ 2016 Tony Su All Rights Reserved #
#                                              #
#----------------------------------------------#

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PalmA_crossplatform
TEMPLATE = app


SOURCES += main.cpp\
    status_widget.cpp

HEADERS += status_widget.h

FORMS   += status_widget.ui

win32{
    SOURCES += operation_win.cpp
    HEADERS += operation_win.h
}

unix{
    SOURCES += operation_unix.cpp
    HEADERS += operation_unix.h
}
