#include "noteswindow.h"
#include "ui_noteswindow.h"
#include <QPushButton>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>

struct note {
    QString nameOfNote;
    QString textOfNote;
    QString dateOfChanges;
    QString dateOfNotification;
};
QString noteLocation = "resources/Notes/";
QString username = "";
QList<note> notesList;

NotesWindow::NotesWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotesWindow)
{
    ui->setupUi(this);
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
    QFile xmlFile(noteLocation + ".xml");
    note buf;

    if(!xmlFile.open(QFile::ReadOnly)) {
        return;
    }

    QXmlStreamReader reader(&xmlFile);
    while(reader.readNextStartElement()) {
        if(reader.name() == "nameOfNote") {
            buf.nameOfNote = reader.readElementText();
        }
        if(reader.name() == "textOfNote") {
            buf.textOfNote = reader.readElementText();
        }
        if(reader.name() == "dateOfChanges") {
            buf.dateOfChanges = reader.readElementText();
        }
        if(reader.name() == "dateOfNotification") {
            buf.dateOfNotification = reader.readElementText();
            notesList.push_back(buf);
        }
    }
}

void NotesWindow::WriteXML() {
    QFile xmlFile(noteLocation + ".xml");
    xmlFile.open(QIODevice::WriteOnly);
    QXmlStreamWriter writer(&xmlFile);

    writer.setAutoFormatting(true);
    writer.setCodec("UTF-8");

    for(int i = 0; i < notesList.size(); i++) {
        writer.writeTextElement("nameOfNote", notesList[i].nameOfNote);
        writer.writeTextElement("textOfNote", notesList[i].textOfNote);
        writer.writeTextElement("dateOfChanges", notesList[i].dateOfChanges);
        writer.writeTextElement("dateOfNotification", notesList[i].dateOfNotification);

    }

    writer.writeEndElement();
    xmlFile.close();
}
