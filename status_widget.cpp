#include "status_widget.h"
#include "ui_status_widget.h"

StatusWidget::StatusWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusWidget)
{
    ui->setupUi(this);
}

StatusWidget::~StatusWidget()
{
    delete ui;
}
