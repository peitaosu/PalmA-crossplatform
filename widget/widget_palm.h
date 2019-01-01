//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2019 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


#ifndef PALMA_WIDGET_PALM_H
#define PALMA_WIDGET_PALM_H

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
        void setAvailable(bool available = true);

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

#endif // PALMA_WIDGET_PALM_H
