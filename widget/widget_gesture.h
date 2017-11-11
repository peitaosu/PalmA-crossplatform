#ifndef PALMA_WIDGET_GESTURE_H
#define PALMA_WIDGET_GESTURE_H

#include <QWidget>

namespace Ui {
    class GestureWidget;
}

class GestureWidget : public QWidget
{
        Q_OBJECT

    public:
        explicit GestureWidget(QWidget *parent = 0);
        ~GestureWidget();
        bool isAvailable();
        void setAvailable(bool available = true);

    public slots:
        void setGestureDisplay(QString gesture_type);

    private:
        Ui::GestureWidget *ui;

        bool available;
};

#endif // PALMA_WIDGET_GESTURE_H
