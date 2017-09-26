#ifndef NOTESWINDOW_H
#define NOTESWINDOW_H

#include <QWidget>

namespace Ui {
class NotesWindow;
}

class NotesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit NotesWindow(QWidget *parent = 0);
    ~NotesWindow();

private:
    Ui::NotesWindow *ui;
};

#endif // NOTESWINDOW_H
