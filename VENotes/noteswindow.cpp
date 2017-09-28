#include "noteswindow.h"
#include "ui_noteswindow.h"
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QDate>
#include <QIODevice>
#include <QTextStream>
#include <QListWidgetItem>

struct note {
    QString nameOfNote;
    QString textOfNote;
    QString dateOfChanges;
    QString dateOfNotification = "0";
    int ID = -1;
};
int openID = -1;
QString noteLocation = "resources/Notes/";
QString username = "pavlenko";
QList<note> notesList;

NotesWindow::NotesWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotesWindow)
{
    ui->setupUi(this);

    showNotes();

    ui->buttonBox_in_reg->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->buttonBox_in_reg->button(QDialogButtonBox::Ok)->setText(tr("Створити"));
    ui->buttonBox_in_reg->button(QDialogButtonBox::Cancel)->setText(tr("Скасувати"));

    QRegExp validExp("\\b[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}\\b");
    validator = new QRegExpValidator(validExp, ui->lineEdit_reg_email);
    ui->lineEdit_reg_email->setValidator(validator);

    connect(ui->lineEdit_reg_log, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToRegOk()));
    connect(ui->lineEdit_reg_pass, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToRegOk()));
    connect(ui->lineEdit_reg_pass_check, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToRegOk()));
    connect(ui->lineEdit_reg_email, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToRegOk()));
    connect( ui->buttonBox_in_reg->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), this, SLOT(createAccount()));
}

NotesWindow::~NotesWindow()
{
    delete ui;
}

void NotesWindow::createAccount()
{
    QString message;
    if(ui->lineEdit_reg_log->text().isEmpty())
        message = "Ви не вказали логін.\n";
    if(ui->lineEdit_reg_pass->text().isEmpty())
        message += "Ви не вказали пароль.\n";
    if(ui->lineEdit_reg_pass_check->text().isEmpty())
        message += "Ви не повторили пароль.\n";
    if(ui->lineEdit_reg_pass->text().compare(ui->lineEdit_reg_pass_check->text()))
        message += "Паролі не співпадають.\n";
    if(ui->lineEdit_reg_log->text().isEmpty())
        message += "Ви не вказали email.\n";
    if(!ui->lineEdit_reg_email->hasAcceptableInput())
        message += "Такий email не може існувати.\n";
    if(!message.isEmpty())
    {
        ui->buttonBox_in_reg->button(QDialogButtonBox::Ok)->setEnabled(false);
        QMessageBox::warning(this, tr("Помилка реєстрації"), message + "Повторіть введення даних!");
    }
}

void NotesWindow::setEnabledToRegOk()
{
    if(!ui->lineEdit_reg_log->text().isEmpty() && !ui->lineEdit_reg_pass->text().isEmpty() && !ui->lineEdit_reg_pass_check->text().isEmpty() && !ui->lineEdit_reg_email->text().isEmpty())
        ui->buttonBox_in_reg->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void NotesWindow::readXML() {
    notesList.clear();
    QFile readFrom(noteLocation + username + ".txt");
    readFrom.open(QIODevice::ReadOnly);
    QString str = "";
    note bufForNotes;
    while(!readFrom.atEnd()) {
        QString buf;
        buf = readFrom.readLine();
        if(buf.contains("<nameOfNote>")) {
            str.clear();
        }
        else if(buf.contains("<textOfNote>")) {
            bufForNotes.nameOfNote = str.mid(0, str.indexOf('\n'));
            str.clear();
        }
        else if(buf.contains("<dateOfChanges>")) {
            bufForNotes.textOfNote = str;
            str.clear();
        }
        else if(buf.contains("<dateOfNotification>")) {
            bufForNotes.dateOfChanges = str.mid(0, str.indexOf('\n'));
            str.clear();
        }
        else if(buf.contains("<endOfNote>")) {
            bufForNotes.dateOfNotification = str.mid(0, str.indexOf('\n'));
            str.clear();
            bufForNotes.ID = notesList.size();
            notesList.push_back(bufForNotes);
        }
        else {
            str += buf;
        }
    }
    readFrom.close();
}

void NotesWindow::WriteXML() {
    QFile writeTo(noteLocation + username + ".txt");
    writeTo.open(QFile::WriteOnly);
    QString fileContain = "";
    for(int i = 0; i < notesList.size(); i++) {
        fileContain += "<nameOfNote>\r\n";
        fileContain += notesList[i].nameOfNote;
        fileContain += "\r\n<textOfNote> \r\n";
        fileContain += notesList[i].textOfNote;
        fileContain += "\r\n<dateOfChanges> \r\n";
        fileContain += notesList[i].dateOfChanges;
        fileContain += "\r\n<dateOfNotification> \r\n";
        fileContain += notesList[i].dateOfNotification;
        fileContain += "\r\n<endOfNote>\r\n";
    }
    QTextStream out(&writeTo);
    out.setCodec("UTF-8");
    out << fileContain;
    writeTo.close();
}

void NotesWindow::on_fontComboBox_activated() {
    ui->notesText->setFont(ui->fontComboBox->currentFont());
}

void NotesWindow::on_spinBox_valueChanged() {
    ui->notesText->setStyleSheet("font: " + QString::number(ui->spinBox->value()) + "pt;");
}

void NotesWindow::showNotes() {
    ui->notesName->setEnabled(false);
    ui->notesText->setEnabled(false);
    ui->spinBox->setEnabled(false);
    ui->fontComboBox->setEnabled(false);
    ui->stackedWidget->setCurrentIndex(2);
    readXML();
    makeListOfNotes();
}


void NotesWindow::on_saveNoteButton_clicked() {
    QDate now = QDate::currentDate();
    note buf;


    buf.nameOfNote = ui->notesName->text();
    for(int i = 0; i < notesList.size(); i++) {
        if(ui->notesName->text() == notesList[i].nameOfNote && openID != notesList[i].ID) {
            QMessageBox::critical(this, "Помилка", "Оберiть оригiнальну назву нотатка!");
            return;
        }
    }
    buf.textOfNote = ui->notesText->toPlainText();
    buf.dateOfChanges = now.toString();
    buf.ID = openID;

    if(buf.ID != -1) {
        notesList[buf.ID].dateOfChanges = buf.dateOfChanges;
        notesList[buf.ID].nameOfNote = buf.nameOfNote;
        notesList[buf.ID].textOfNote = buf.textOfNote;
    }
    else {
        buf.ID = notesList.size();
        openID = buf.ID;
        notesList.push_back(buf);
    }

    WriteXML();
    makeListOfNotes();
}

void NotesWindow::makeListOfNotes() {
    ui->notesShowList->clear();
    for(int i = 0; i < notesList.size(); i++) {
        QString obj = notesList[i].nameOfNote + "\r\n";
        QString comp = notesList[i].textOfNote.mid(0, notesList[i].textOfNote.indexOf('\n'));
        for(int j = 0; j < 20; j++) {
                obj += comp[j];
        }
        if(comp.size() > 20)
            obj += "...";
        ui->notesShowList->addItem(obj);
    }
}

void NotesWindow::on_notesShowList_itemDoubleClicked() {
   QString name = ui->notesShowList->currentItem()->text();
   int num = name.indexOf('\n');
   QString nameText = name.mid(0, num - 1);
   for(int i = 0; i < notesList.size(); i++) {
       if(notesList[i].nameOfNote == nameText) {
           ui->notesName->setEnabled(true);
           ui->notesText->setEnabled(true);
           ui->spinBox->setEnabled(true);
           ui->fontComboBox->setEnabled(true);

           ui->notesName->setText(notesList[i].nameOfNote);
           ui->notesText->setText(notesList[i].textOfNote);
           openID = notesList[i].ID;
           break;
       }
   }
}

void NotesWindow::on_newNoteButton_clicked() {
    ui->notesName->setEnabled(true);
    ui->notesName->setText("");
    ui->notesText->setEnabled(true);
    ui->notesText->setText("");
    ui->spinBox->setEnabled(true);
    ui->fontComboBox->setEnabled(true);
    openID = -1;
}


void NotesWindow::on_deleteNoteButton_clicked() {
    for(int i = 0; i < notesList.size(); i++) {
        if(openID == notesList[i].ID) {
            notesList.erase(notesList.begin() + i);
        }
    }
    openID = -1;
    ui->notesName->setText("");
    ui->notesText->setText("");
    ui->notesName->setEnabled(false);
    ui->notesText->setEnabled(false);
    ui->spinBox->setEnabled(false);
    ui->fontComboBox->setEnabled(false);

    WriteXML();
    makeListOfNotes();
}
