#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

class NotesWindow;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(NotesWindow *notes, QWidget *parent = 0);
    void setName(QString);
    void setLabels(QString, QString);
    void setPasswordFlag();
    void setEmailFlag();
    ~Dialog();

private:
    Ui::Dialog *ui;
    NotesWindow * notes;
    bool isEmail;

private slots:
    void returnData();
    void setEnabledToOk();

    void on_buttonBox_accepted();

signals:
    void refreshEmailSignal();
    void refreshPasswordSignal();
};

#endif // DIALOG_H
