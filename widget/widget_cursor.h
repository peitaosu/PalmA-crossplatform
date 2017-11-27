//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2017 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


#ifndef PALMA_WIDGET_CURSOR_H
#define PALMA_WIDGET_CURSOR_H

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

#endif // PALMA_WIDGET_CURSOR_H
