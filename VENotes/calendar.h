#ifndef CALENDAR_H
#define CALENDAR_H

#include <QDialog>
#include "noteswindow.h"

namespace Ui {
class Calendar;
}

class NotesWindow;

class Calendar : public QDialog
{
    Q_OBJECT

public:
    explicit Calendar(QWidget *parent = 0);
    ~Calendar();

private:
    Ui::Calendar *ui;
    NotesWindow * parent;
private slots:
    void on_CancelButton_clicked();
    void on_OkButton_clicked();
};

#endif // CALENDAR_H
