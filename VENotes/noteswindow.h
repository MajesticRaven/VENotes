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

    void readXML();

    void WriteXML();

    void on_fontComboBox_activated();

    void on_spinBox_valueChanged();

    void showNotes();

    void on_saveNoteButton_clicked();

    void makeListOfNotes();

    void on_notesShowList_itemDoubleClicked();

    void on_newNoteButton_clicked();

    void on_deleteNoteButton_clicked();

    void createMenu();

    void exitButton();

    void changeAccount();

    void on_notificationButton_clicked();

    void on_CancelButton_clicked();

    void on_OkButton_clicked();

    void on_SearchButton_clicked();

private:
    Ui::NotesWindow *ui;
    QValidator * validator;
};

#endif // NOTESWINDOW_H
