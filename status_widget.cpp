#include "status_widget.h"
#include "ui_status_widget.h"

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
