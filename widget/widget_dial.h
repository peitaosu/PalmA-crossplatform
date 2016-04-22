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
        QString getChoose();
        bool isAvailable();

    public slots:
        //position
        void setPosition(double x, double y);
        void setTargetPosition(double x, double y);

        //dial
        void setDial(QString dial_type);
        void doneDial();

        //pixmap
        void setPixmap(QPixmap pixmap_type);

        //available
        void setAvailable(bool available = true);

    signals:
        //choose
        void choose_up();
        void choose_down();
        void choose_left();
        void choose_right();
        void choose_up(double, double);
        void choose_down(double, double);
        void choose_left(double, double);
        void choose_right(double, double);
        
    private:
        Ui::DialWidget *ui;
        QPixmap pixmap;

        double position_x;
        double position_y;
        double target_x;
        double target_y;

        bool available;
        

};

#endif // WIDGET_DIAL_H
