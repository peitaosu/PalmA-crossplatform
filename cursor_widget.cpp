#include "cursor_widget.h"
#include "ui_cursor_widget.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPalette>
#include <QColor>
#include <QPainter>
#include <QPoint>
#include <QTimer>

#ifdef Q_OS_WIN32
#include "windows.h"
#endif

#ifdef Q_OS_LINUX
#include <X11/extensions/shape.h>
#include <QtX11Extras/QX11Info>
#endif

CursorWidget::CursorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CursorWidget)
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

    //new timer named cursor_paint_timer, used to update display
    QTimer *cursor_paint_timer = new QTimer(this);
    connect(cursor_paint_timer,SIGNAL(timeout()),this,SLOT(update()));
    cursor_paint_timer->start(20);

    //set cursor color
    cursor_color = QColor(00, 160, 255, 200);

    //set cursor available
    available = true;
}

CursorWidget::~CursorWidget()
{
    delete ui;
}

bool CursorWidget::setCursorColor(int r, int g, int b)
{
    if(r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0){
        return false;
    }
    cursor_color = QColor(00, r, g, b);
    return true;
}

bool CursorWidget::isAvailable()
{
    if(available){
        return true;
    }else{
        return false;
    }
}

void CursorWidget::setAvailable(bool _available)
{
    available = _available;
}

void CursorWidget::paintEvent(QPaintEvent *event){
    if(available){
        //convert the hand position to the screen
        i_box = controller.frame().interactionBox();
        cursor_x = i_box.normalizePoint(controller.frame().hands()[0].stabilizedPalmPosition()).x;
        cursor_y = i_box.normalizePoint(controller.frame().hands()[0].stabilizedPalmPosition()).y;

        //cursor point in screen
        int screen_x = cursor_x * QApplication::desktop()->width();
        int screen_y = (1 - cursor_y) * QApplication::desktop()->height();

        //new painter
        QPainter painter(this);

        //Clear
        painter.fillRect(this->rect(), QColor(0,0,0,0));

        //Set Pen and Brush, Brush color is R160:G255:B200 (BLUE)
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(cursor_color));

        //Draw Ellipse, size is 30*30, position is (screen_x - 15, screen_y - 15)
        painter.drawEllipse(screen_x-15 ,screen_y-15,30,30);
    }
}
