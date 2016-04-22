#ifndef WIDGET_CURSOR_H
#define WIDGET_CURSOR_H

#include <QWidget>
#include <QColor>
#include "include/Leap.h"

namespace Ui {
    class CursorWidget;
}

class CursorWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit CursorWidget(QWidget *parent = 0);
        ~CursorWidget();
        bool setCursorColor(int r, int g, int b);
        void updateCursor(double x, double y);
        bool isAvailable();
        void setAvailable(bool available = true);

    public slots:
        void paintEvent(QPaintEvent *event);

    private:
        Ui::CursorWidget *ui;
        Leap::Controller controller;
        Leap::InteractionBox i_box;
        QColor cursor_color;

        double cursor_x;
        double cursor_y;
        bool available;
};

#endif // WIDGET_CURSOR_H
