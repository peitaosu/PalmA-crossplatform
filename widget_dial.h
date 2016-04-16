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

        void changeDial(int dial_type);
        void setPixmap(QPixmap pixmap_type);

    private:
        Ui::DialWidget *ui;
        QPixmap pixmap;
};

#endif // WIDGET_DIAL_H
