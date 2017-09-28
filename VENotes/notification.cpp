#include "notification.h"
#include "ui_notification.h"

Notification::Notification(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Notification)
{
    ui->setupUi(this);
}

Notification::~Notification()
{
    delete ui;
}

void Notification::on_CancelButton_clicked()
{
    this->close();
    this->deleteLater();
}

void Notification::on_OkButton_clicked()
{

}
