#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Machine machine;
    MainWindow w(&machine);
    w.show();
    return a.exec();
}
