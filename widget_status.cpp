#include "widget_status.h"
#include "ui_widget_status.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QPalette>

#ifdef Q_OS_WIN32
#include "windows.h"
#endif

#ifdef Q_OS_LINUX
#include <X11/extensions/shape.h>
#include <QtX11Extras/QX11Info>
#endif

StatusWidget::StatusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusWidget)
{
    ui->setupUi(this);

    //set cursor available
    available = false;
    this->hide();

    //Set Geometry (-60,-60) from Right-Buttom, size is 180*30
    this->setGeometry(QApplication::desktop()->width()-240,QApplication::desktop()->height()-285,180,30);

    //Set Window Background Color R0:G0:B0 (BLACK), Opacity is 0.8
    this->setPalette(QPalette( QColor(0, 0, 0)) );
    this->setWindowOpacity(0.8);

    //Set Window Flag : Tool | Frameless | Stay On Top
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

}

StatusWidget::~StatusWidget()
{
    delete ui;
}

bool StatusWidget::isAvailable()
{
    return available;
}

void StatusWidget::setAvailable(bool _available)
{
    available = _available;
    if(available == true){
       this->show();
    }else{
        this->hide();
    }
}

void StatusWidget::setStatusDisplay(int status_code)
{
    switch(status_code){
        case 0:
            this->setStyleSheet("#frame_status1{background-color: rgb(0, 0, 0);}#frame_status2{background-color: rgb(0, 0, 0);}#frame_status3{background-color: rgb(0, 0, 0);}");
            break;
        case 1:
            this->setStyleSheet("#frame_status1{background-color: rgb(0, 0, 0);}#frame_status2{background-color: rgb(0, 0, 0);}#frame_status3{background-color: rgb(0, 0, 0);}");
            break;
        case 2:
            this->setStyleSheet("#frame_status1{background-color: rgb(0, 0, 0);}#frame_status2{background-color: rgb(0, 240, 0);}#frame_status3{background-color: rgb(0, 0, 0);}");
            break;
        case 3:
            this->setStyleSheet("#frame_status1{background-color: rgb(0, 0, 0);}#frame_status2{background-color: rgb(0, 240, 0);}#frame_status3{background-color: rgb(0, 0, 0);}");
            break;
        case 4:
            this->setStyleSheet("#frame_status1{background-color: rgb(0, 240, 0);}#frame_status2{background-color: rgb(0, 0, 0);}#frame_status3{background-color: rgb(0, 0, 0);}");
            break;
        case 5:
            this->setStyleSheet("#frame_status1{background-color: rgb(0, 240, 0);}#frame_status2{background-color: rgb(0, 0, 0);}#frame_status3{background-color: rgb(0, 240, 0);}");
            break;
        case 6:
            this->setStyleSheet("#frame_status1{background-color: rgb(0, 240, 0);}#frame_status2{background-color: rgb(0, 240, 0);}#frame_status3{background-color: rgb(0, 0, 0);}");
            break;
        case 7:
            this->setStyleSheet("#frame_status1{background-color: rgb(0, 240, 0);}#frame_status2{background-color: rgb(0, 240, 0);}#frame_status3{background-color: rgb(0, 240, 0);}");
            break;
        default:
            break;
    }
}
