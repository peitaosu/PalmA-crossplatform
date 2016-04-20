#include "widget_palm.h"
#include "ui_widget_palm.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPalette>
#include <QColor>
#include <QPainter>
#include <QPoint>
#include <QTimer>
#include <cmath>

#ifdef Q_OS_WIN32
#include "windows.h"
#endif

#ifdef Q_OS_LINUX
#include <X11/extensions/shape.h>
#include <QtX11Extras/QX11Info>
#endif

PalmWidget::PalmWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PalmWidget)
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

    //new timer named palm_paint_timer, used to update display
    QTimer *palm_paint_timer = new QTimer(this);
    connect(palm_paint_timer,SIGNAL(timeout()),this,SLOT(update()));
    palm_paint_timer->start(20);

    //set palm color
    palm_color = QColor(00, 160, 255, 200);

    //set palm available
    available = true;

    //init interaction box
    i_box = controller.frame().interactionBox();
    controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

    this->show();

}

PalmWidget::~PalmWidget()
{
    delete ui;
}

bool PalmWidget::setPalmColor(int r, int g, int b)
{
    if(r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0){
        return false;
    }
    palm_color = QColor(00, r, g, b);
    return true;
}

bool PalmWidget::isAvailable()
{
    if(available){
        return true;
    }else{
        return false;
    }
}

void PalmWidget::setAvailable(bool _available)
{
    available = _available;
}

void PalmWidget::paintEvent(QPaintEvent *event){
    if(available){

        //convert the hand position to the screen
        i_box = controller.frame().interactionBox();
        palm_x = i_box.normalizePoint(controller.frame().hands()[0].stabilizedPalmPosition()).x;
        palm_y = 1 - i_box.normalizePoint(controller.frame().hands()[0].stabilizedPalmPosition()).y;
        int screen_x = QApplication::desktop()->width() * palm_x;
        int screen_y = QApplication::desktop()->height() * palm_y;

        //get bone closest to the finger tip each finger
        double bone_tip[5][2];
        for(int finger = 0; finger < 5; finger++){
            bone_tip[finger][0] = i_box.normalizePoint(controller.frame().hands()[0].fingers()[finger].bone(static_cast<Leap::Bone::Type>(3)).nextJoint()).x;
            bone_tip[finger][1] = 1 - i_box.normalizePoint(controller.frame().hands()[0].fingers()[finger].bone(static_cast<Leap::Bone::Type>(3)).nextJoint()).y;
        }

        //convert to screen position
        int tip[5][2];

        if(palm_x != 0 && palm_y != 1){
            for(int finger = 0; finger < 5; finger++){
                int x = QApplication::desktop()->width() * bone_tip[finger][0];
                int y = QApplication::desktop()->height() * bone_tip[finger][1];
                //fingers follow by the palm
                tip[finger][0] = 88*(x - screen_x)/(sqrt(pow(x - screen_x, 2)+pow(y - screen_y, 2))) + screen_x;
                tip[finger][1] = 88*(y - screen_y)/(sqrt(pow(x - screen_x, 2)+pow(y - screen_y, 2))) + screen_y;
            }
            //std::cout<<tip[0][0]<<std::endl;
        }

        //new painter
        QPainter painter(this);

        //clear
        painter.fillRect(this->rect(), QColor(0,0,0,0));

        //set Pen and Brush, Brush color is R160:G255:B200 (BLUE)
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(palm_color));

        //draw Ellipse, size is 128*128, position is (screen_x - 64, screen_y - 64)
        painter.drawEllipse(screen_x - 64, screen_y - 64, 128, 128);

        //draw fingers ellipse, size is 32*32, circle the palm
        for(int finger = 0; finger < 5; finger++){
            painter.drawEllipse(tip[finger][0] - 16, tip[finger][1] - 16, 32, 32);
        }

    }
}
