#include "calendar.h"
#include "ui_calendar.h"
#include "ui_noteswindow.h"
#include <QLocale>

Calendar::Calendar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Calendar)
{
    this->parent = (NotesWindow *)parent;
    ui->setupUi(this);
    this->setWindowTitle(tr("Нагадати про подію"));
    this->setFixedSize(this->sizeHint());
}

Calendar::~Calendar()
{
    delete ui;
    ui->calendarWidget->setLocale(QLocale::Ukrainian);
}

void Calendar::on_CancelButton_clicked()
{
    close();
}

void Calendar::on_OkButton_clicked()
{
    parent->setDate(ui->calendarWidget->selectedDate().toString());
    close();
}
