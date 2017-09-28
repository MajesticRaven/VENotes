#include "noteswindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NotesWindow w;

    QFile qss("resources/style.qss");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());
    a.topLevelWindows();

    w.setWindowTitle("VENotes");
    w.show();

    return a.exec();
}
