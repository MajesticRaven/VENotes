#ifndef NOTESWINDOW_H
#define NOTESWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QDate>
#include <QIODevice>
#include <QTextStream>
#include <QListWidgetItem>
#include <QMenu>
#include <QAction>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlResult>
#include <QCryptographicHash>
#include <QTextBrowser>
#include <QScrollBar>
#include <QSettings>
#include <QIcon>
class QValidator;
class QRegExp;
class QSqlDatabase;

class QValidator;

namespace Ui {
class NotesWindow;
}

class NotesWindow : public QWidget
{
    Q_OBJECT

public:
    explicit NotesWindow(QWidget *parent = 0);
    static bool connectDB();
    ~NotesWindow();

private slots:
    void createAccount();

    void loginInAccount();

    void setEnabledToRegOk();

    void setEnabledToAuthOk();

    void readXML();

    void WriteXML();

    void on_pushButton_new_account_clicked();

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

    void on_pushButton_auth_clicked();

    void readSettings();

    void writeSettings();
private:
    Ui::NotesWindow *ui;
    QValidator * validator;

    void registration();
    void authorization();
};

#endif // NOTESWINDOW_H
