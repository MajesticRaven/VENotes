#include "finddialog.h"
#include "ui_finddialog.h"
#include <QSize>
#include <noteswindow.h>

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this); 
    this->setFixedHeight(this->sizeHint().height());
    this->setMinimumWidth(this->sizeHint().width() + 80);
    this->notes = (NotesWindow *)parent;
    ui->ok_search->setEnabled(false);

    connect(ui->lineEdit_text_for_search, SIGNAL(textChanged(QString)), this, SLOT(setEnabledToOk()));
}

FindDialog::~FindDialog()
{
    delete ui;
}
void FindDialog::setEnabledToOk()
{
    if(ui->lineEdit_text_for_search->text().isEmpty())
        ui->ok_search->setEnabled(false);
    else ui->ok_search->setEnabled(true);
}
void FindDialog::returnData()
{
    notes->search(ui->lineEdit_text_for_search->text(), ui->checkBox_search->isChecked(), ui->checkBox_search_2->isChecked());
}
void FindDialog::on_ok_search_clicked()
{
    returnData();
    close();
}
