#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
class NotesWindow;

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = 0);
    ~FindDialog();

private:
    Ui::FindDialog *ui;
    NotesWindow * notes;

private slots:
    void setEnabledToOk();
    void on_ok_search_clicked();
    void returnData();
};

#endif // FINDDIALOG_H
