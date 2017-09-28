#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>

namespace Ui {
class Notification;
}

class Notification : public QWidget
{
    Q_OBJECT

public:
    explicit Notification(QWidget *parent = 0);
    ~Notification();

private slots:
    void on_CancelButton_clicked();

    void on_OkButton_clicked();

private:
    Ui::Notification *ui;
};

#endif // NOTIFICATION_H
