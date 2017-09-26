#include "noteswindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NotesWindow w;
    w.show();
//Проверка
    return a.exec();
}
