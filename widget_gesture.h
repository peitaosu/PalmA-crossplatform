#ifndef WIDGET_GESTURE_H
#define WIDGET_GESTURE_H

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
        void setAvailable(bool _available = true);

    public slots:
        void setGestureDisplay(int gesture_code);

    private:
        Ui::GestureWidget *ui;

        bool available;
};

#endif // WIDGET_GESTURE_H
