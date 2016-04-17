#include "widget_dial.h"
#include "ui_widget_dial.h"
#include <QBitmap>
#include <QPalette>
#include <QPainter>
#include <QPen>
#include <QPaintEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <QDebug>

#ifdef Q_OS_WIN32
#include "windows.h"
#endif

#ifdef Q_OS_LINUX
#include <X11/extensions/shape.h>
#include <QtX11Extras/QX11Info>
#endif

DialWidget::DialWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialWidget)
{
    ui->setupUi(this);

    //get the user device screen size
    int user_device_screen_width = QApplication::desktop()->width();
    int user_device_screen_height= QApplication::desktop()->height();

    //set window geometry size is full screen
    this->setGeometry(0, 0, user_device_screen_width, user_device_screen_height);

    //Translucent Background
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    //set window flag are frameless and stay-on-top
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

    //update dial
    QTimer *updateDial = new QTimer(this);
    connect(updateDial,SIGNAL(timeout()),this,SLOT(update()));
    updateDial->start(20);

    //set to unvisible
    this->setVisible(false);

}

DialWidget::~DialWidget()
{
    delete ui;
}

void DialWidget::paintEvent(QPaintEvent *event)
{
        QPainter painter(this);
        QPen pen;
        pen.setStyle(Qt::SolidLine);
        pen.setWidth (30);
        pen.setCapStyle(Qt::PenCapStyle(Qt::RoundCap));
        pen.setColor(QColor(00,160,255,200));
        painter.setPen(pen);
        painter.setBrush(QBrush(QColor(00,160,255,200)));
        int user_desktop_width = QApplication::desktop()->width();
        int user_desktop_height = QApplication::desktop()->height();
        int begin_x = position_x * user_desktop_width;
        int begin_y = position_y * user_desktop_height;
        int end_x = target_x * user_desktop_width;
        int end_y = target_y * user_desktop_height;
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.drawPixmap(begin_x - 160,begin_y - 160,320,320, pixmap);
        painter.drawLine( begin_x, begin_y, end_x, end_y);
        //this->setWindowFlags(Qt::WindowStaysOnTopHint);
}

void DialWidget::setPosition(double x, double y)
{
    position_x = x;
    position_y = y;
}

void DialWidget::setTargetPosition(double x, double y)
{
    target_x = x;
    target_y = y;
}

void DialWidget::setDial(QString dial_type)
{
    QString dial_resource = ":/resource/dial/dial_" + dial_type + ".png";
    //QImage image(dial_resource);
    //qDebug()<<image.save(dial_resource, "PNG");
    QPixmap _pixmap;
    _pixmap.load(dial_resource);
    this->setPixmap(_pixmap);
}

void DialWidget::setPixmap(QPixmap pixmap_type)
{
    pixmap = pixmap_type;
}


