#include "widget_status.h"
#include "ui_widget_status.h"

StatusWidget::StatusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusWidget)
{
    ui->setupUi(this);
    available = true;
}

StatusWidget::~StatusWidget()
{
    delete ui;
}

bool StatusWidget::isAvailable()
{
    return available;
}

void StatusWidget::setAvailable(bool _available)
{
    available = _available;
}
