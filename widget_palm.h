#ifndef WIDGET_PALM_H
#define WIDGET_PALM_H

#include <QWidget>
#include <QColor>
#include "include/Leap.h"

namespace Ui {
    class PalmWidget;
}

class PalmWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit PalmWidget(QWidget *parent = 0);
        ~PalmWidget();
        bool setPalmColor(int r, int g, int b);
        bool isAvailable();
        void setAvailable(bool _available = true);

    public slots:
        void paintEvent(QPaintEvent *event);

    private:
        Ui::PalmWidget *ui;
        Leap::Controller controller;
        Leap::InteractionBox i_box;
        QColor palm_color;

        double palm_x;
        double palm_y;
        bool available;
};

#endif // WIDGET_PALM_H
