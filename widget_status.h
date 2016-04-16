#ifndef WIDGET_STATUS_H
#define WIDGET_STATUS_H

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

    public slots:
        void setStatusDisplay(int status_code);

    private:
        Ui::StatusWidget *ui;

        bool available;
};

#endif // WIDGET_STATUS_H
