//---------------------------------------------------//
//     Apache License Version 2.0, January 2004      //
// Copyright @ 2016-2017 Tony Su All Rights Reserved //
//  https://github.com/peitaosu/PalmA-crossplatform  //
//---------------------------------------------------//


#ifndef PALMA_WIDGET_STATUS_H
#define PALMA_WIDGET_STATUS_H

#include <QWidget>
namespace Ui {
    class StatusWidget;
}

class StatusWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit StatusWidget(QWidget *parent = 0);
        ~StatusWidget();
        bool isAvailable();
        void setAvailable(bool available = true);

    public slots:
        void setStatusDisplay(int status_code);

    private:
        Ui::StatusWidget *ui;

        bool available;
};

#endif // PALMA_WIDGET_STATUS_H
