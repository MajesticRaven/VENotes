#ifndef NOTESWINDOW_H
#define NOTESWINDOW_H

#include <QMainWindow>

namespace Ui {
class NotesWindow;
}

class NotesWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NotesWindow(QWidget *parent = 0);
    ~NotesWindow();

private:
    Ui::NotesWindow *ui;
};

#endif // NOTESWINDOW_H
