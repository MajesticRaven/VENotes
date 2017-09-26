#include "noteswindow.h"
#include "ui_noteswindow.h"

NotesWindow::NotesWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotesWindow)
{
    ui->setupUi(this);
}

NotesWindow::~NotesWindow()
{
    delete ui;
}
