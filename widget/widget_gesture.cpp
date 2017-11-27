//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2017 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


#include "widget_gesture.h"
#include "ui_widget_gesture.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPalette>
#include "motion_code.h"
#ifdef Q_OS_WIN32
#include "windows.h"
#endif

#ifdef Q_OS_LINUX
#include <X11/extensions/shape.h>
#include <QtX11Extras/QX11Info>
#endif
#include<iostream>
GestureWidget::GestureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GestureWidget)
{
    ui->setupUi(this);

    //set cursor available
    available = true;

    //set Geometry (-60,-60) from Right-Buttom, size is 180*180
    this->setGeometry(QApplication::desktop()->width()-240,QApplication::desktop()->height()-240,180,180);

    //set Window Background Color R0:G0:B0 (BLACK), Opacity is 0.8
    this->setPalette(QPalette( QColor(0, 0, 0)) );
    this->setWindowOpacity(0.8);

    //set Window Flag : Tool | Frameless | Stay On Top
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    //TRANSPARENT and LAYERED for Windows
    #ifdef Q_OS_WIN32
        SetWindowLong((HWND)winId(), GWL_EXSTYLE, GetWindowLong((HWND)winId(), GWL_EXSTYLE) |
                      WS_EX_TRANSPARENT | WS_EX_LAYERED);
    #endif

    //for Linux/X11
    #ifdef Q_OS_LINUX
        XShapeCombineRectangles(QX11Info::display(), winId(), ShapeInput,0,0, NULL, 0, ShapeSet, YXBanded);
    #endif

    this->show();
}

GestureWidget::~GestureWidget()
{
    delete ui;
}

/*
 * Gesture: Set Gesture Display
 * Input: QString gesture type, name same as file name
 * Return: NONE
 */
void GestureWidget::setGestureDisplay(QString gesture_type){
    //the resource path
    QString picture_path = "\":/resource/gesture/";
    QString picture_name = gesture_type;
    //set up the style sheet
    QString gesture_label_css = "#gesture_show_label{border-image: url(" + picture_path + picture_name + ".png\");}";
    this->setStyleSheet(gesture_label_css);
}

/*
 * Gesture: Get the Gesture Available
 * Input: NONE
 * Return: bool available or not
 */
bool GestureWidget::isAvailable(){
    return available;
}

/*
 * Gesture: Set the Gesture Available
 * Input: bool available or not
 * Return: NONE
 */
void GestureWidget::setAvailable(bool available){
    this->available = available;
    if(this->available == true){
       this->show();
    }else if(this->available == false){
       this->hide();
    }
}

