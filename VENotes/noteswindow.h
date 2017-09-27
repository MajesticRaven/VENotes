#ifndef NOTESWINDOW_H
#define NOTESWINDOW_H

#include <QWidget>
class QValidator;

namespace Ui {
class NotesWindow;
}

class NotesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit NotesWindow(QWidget *parent = 0);
    ~NotesWindow();
private slots:
    void createAccount();
    void setEnabledToRegOk();

private:
    Ui::NotesWindow *ui;
    QValidator * validator;
};

#endif // NOTESWINDOW_H
