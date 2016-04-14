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
        bool isAvailable();
        void setAvailable(bool _available = true);

    private:
        Ui::StatusWidget *ui;

        bool available;
};

#endif // STATUS_WIDGET_H
