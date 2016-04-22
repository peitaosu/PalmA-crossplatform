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
#include <iostream>
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

    //set to available
    available = false;

}

DialWidget::~DialWidget()
{
    delete ui;
}

/*
 * Dial: Paint the Dial
 * Input: QPaintEvent
 * Return: NONE
 */
void DialWidget::paintEvent(QPaintEvent *event)
{
    if(available == true){
        QPainter painter(this);
        QPen pen;
        //set up the dial line to solid line, width is 30, color is 160,255,200(BLUE)
        pen.setStyle(Qt::SolidLine);
        pen.setWidth (30);
        pen.setCapStyle(Qt::PenCapStyle(Qt::RoundCap));
        pen.setColor(QColor(00,160,255,200));
        painter.setPen(pen);
        painter.setBrush(QBrush(QColor(00,160,255,200)));

        //convert the positions to screen positions
        int user_desktop_width = QApplication::desktop()->width();
        int user_desktop_height = QApplication::desktop()->height();
        int begin_x = position_x * user_desktop_width;
        int begin_y = position_y * user_desktop_height;
        int end_x = target_x * user_desktop_width;
        int end_y = target_y * user_desktop_height;

        //paint the dial, size is 320*320, position in the center, pixmap load from resource
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.drawPixmap(begin_x - 160,begin_y - 160,320,320, pixmap);

        //paint the line
        painter.drawLine( begin_x, begin_y, end_x, end_y);
    }
}

/*
 * Dial: Set the Position
 * Input: double x, double y to position_x and position_y
 * Return: NONE
 */
void DialWidget::setPosition(double x, double y){
    position_x = x;
    position_y = y;
}

/*
 * Dial: Set the Target Position
 * Input: double x, double y to target_x and target_y
 * Return: NONE
 */
void DialWidget::setTargetPosition(double x, double y){
    target_x = x;
    target_y = y;
}

/*
 * Dial: Set the Dial Pixmap File
 * Input: QString dial type, type same as file name
 * Return: NONE
 */
void DialWidget::setDial(QString dial_type){
    //load the pixmap
    QString dial_resource = ":/resource/dial/dial_" + dial_type + ".png";
    QPixmap pixmap;
    pixmap.load(dial_resource);
    this->setPixmap(pixmap);
}

/*
 * Dial: Set the Dial Pixmap
 * Input: QString dial type, type same as file name
 * Return: NONE
 */
void DialWidget::setPixmap(QPixmap pixmap_type){
    pixmap = pixmap_type;
}

/*
 * Dial: Set the Dial Available
 * Input: bool available or not
 * Return: NONE
 */
void DialWidget::setAvailable(bool available){
    this->available = available;
    if(this->available == true){
       this->show();
    }else if(this->available == false){
       this->hide();
    }
}


/*
 * Dial: Release the Dial
 * Input: NONE
 * Return: NONE
 */
void DialWidget::doneDial(){
    //get dial choose
    QString choose = getChoose();

    //up
    if(choose == "up"){
        emit choose_up();
        emit choose_up(position_x, position_y);

    //down
    }else if(choose == "down"){
        emit choose_down();
        emit choose_down(position_x, position_y);

    //left
    }else if(choose == "left"){
        emit choose_left();
        emit choose_left(position_x, position_y);

    //right
    }else if(choose == "right"){
        emit choose_right();
        emit choose_right(position_x, position_y);
    }
}

/*
 * Dial: Get the Dial Choose
 * Input: NONE
 * Return: QString choose, computed with position and target position
 */
QString DialWidget::getChoose(){

    //get slope, (target_y - position_y)/(target_x - position_x)
    double slope;
    if(target_x != position_x){
        slope = (target_y - position_y)/(target_x - position_x);
    }

    //get direction in horizontal and vertical
    int horizontal, vertical = 0;
    if(target_x - position_x >= 0){
        horizontal = 1;
    }else{
        horizontal = -1;
    }
    if(target_y - position_y >= 0){
        vertical = -1;
    }else{
        vertical = 1;
    }

    //get direction of choose
    if(slope >= 1 || slope <= -1){
        if(vertical == 1){
            return "up";
        }else if(vertical == -1){
            return "down";
        }
    }else{
        if(horizontal == 1){
            return "right";
        }else if(horizontal == -1){
            return "left";
        }
    }
    return "none";
}

/*
 * Dial: Get the Dial Available
 * Input: NONE
 * Return: bool available or not
 */
bool DialWidget::isAvailable(){
    return available;
}

