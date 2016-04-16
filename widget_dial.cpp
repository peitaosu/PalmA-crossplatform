#include "widget_dial.h"
#include "ui_widget_dial.h"
#include <QBitmap>
#include <QPalette>
#include <QPaintEvent>
#include <QApplication>
#include <QDesktopWidget>
DialWidget::DialWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialWidget)
{
    ui->setupUi(this);
    //Set Geometry (-60,-60) from Right-Buttom, size is 180*180
    this->setGeometry(QApplication::desktop()->width()-480,QApplication::desktop()->height()-480,400,400);

    //Set Window Background Color R0:G0:B0 (BLACK), Opacity is 0.8
    this->setPalette(QPalette( QColor(0, 0, 0)) );
    this->setWindowOpacity(0.8);

    //Set Window Flag : Tool | Frameless | Stay On Top
    this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    //TODO: load pixmap
}

DialWidget::~DialWidget()
{
    delete ui;
}

void DialWidget::paintEvent(QPaintEvent *event)
{
    QPalette background_palette = this->palette();
    background_palette.setBrush(QPalette::Background,pixmap);
    this->setPalette(background_palette);
}

void DialWidget::changeDial(int dial_type)
{
    //TODO: dial change
}

void DialWidget::setPixmap(QPixmap pixmap_type)
{
    pixmap = pixmap_type;
}


