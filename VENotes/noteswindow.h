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
<<<<<<< HEAD
    void noteWindow();
    void ReadXML(List<note> &litOfNotes);
=======
>>>>>>> 59ed8b81ef4c41685d5979709dcca6c5d865506f

private:
    Ui::NotesWindow *ui;
    QValidator * validator;
};

#endif // NOTESWINDOW_H
