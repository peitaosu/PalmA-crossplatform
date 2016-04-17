#ifndef WIDGET_DIAL_H
#define WIDGET_DIAL_H

#include <QWidget>

namespace Ui {
    class DialWidget;
}

class DialWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit DialWidget(QWidget *parent = 0);
        ~DialWidget();

        void paintEvent(QPaintEvent *event);

        void setPosition(double x, double y);
        void setTargetPosition(double x, double y);
        void setDial(QString dial_type);
        void setPixmap(QPixmap pixmap_type);

    private:
        Ui::DialWidget *ui;
        QPixmap pixmap;

        double position_x;
        double position_y;
        double target_x;
        double target_y;

};

#endif // WIDGET_DIAL_H
