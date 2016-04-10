#ifndef STATUS_WIDGET_H
#define STATUS_WIDGET_H

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

    private:
        Ui::StatusWidget *ui;
};

#endif // STATUS_WIDGET_H
