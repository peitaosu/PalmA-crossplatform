#ifndef DISPLAY_H
#define DISPLAY_H

#include <QObject>
#include "status_widget.h"

class Display : public QObject
{
        Q_OBJECT
    public:
        explicit Display(QObject *parent = 0);

    signals:

    public slots:

    private:
        StatusWidget *status_widget;
};

#endif // DISPLAY_H
