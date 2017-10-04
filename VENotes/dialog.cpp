#include "dialog.h"
#include "ui_dialog.h"
#include <QPushButton>
#include <QValidator>
#include <QRegExp>
#include <noteswindow.h>

Dialog::Dialog(NotesWindow * notes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    isEmail = false;
    ui->setupUi(this);
    this->setFixedSize(this->sizeHint());
    this->notes = notes;
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Змінити"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Скасувати"));
    ui->lineEdit_1->setFixedWidth(180);
    ui->lineEdit_2->setFixedWidth(180);
    connect(ui->lineEdit_1, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToOk()));
    connect(ui->lineEdit_2, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToOk()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setEnabledToOk()
{
    if(isEmail)
    {
        if(ui->lineEdit_1->hasAcceptableInput() && !ui->lineEdit_2->text().isEmpty())
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        else
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else
    {
        if(!ui->lineEdit_1->text().isEmpty() && !ui->lineEdit_2->text().isEmpty())
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        else
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}
void Dialog::setName(QString name)
{
    this->setWindowTitle(name);
    ui->label_name->setText(name);
}
void Dialog::setLabels(QString label1, QString label2)
{
    ui->label_1->setText("&" + label1);
    ui->label_2->setText("&" + label2);
}
void Dialog::setPasswordFlag()
{
    ui->lineEdit_1->setEchoMode(QLineEdit::Password);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
}
void Dialog::setEmailFlag()
{
    QRegExp validExp("\\b[a-zA-Z0-9]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}\\b");
    QValidator * validator = new QRegExpValidator(validExp, ui->lineEdit_1);
    ui->lineEdit_1->setValidator(validator);
    isEmail = true;
    ui->lineEdit_1->setEchoMode(QLineEdit::Normal);
}
void Dialog::returnData()
{
    if(isEmail)
        notes->getDataFromDialog(ui->lineEdit_1->text(), ui->lineEdit_2->text(), true);
    else notes->getDataFromDialog(ui->lineEdit_1->text(), ui->lineEdit_2->text(), false);
}

void Dialog::on_buttonBox_accepted()
{
    returnData();
    close();
}
