#include "mainwindow/mainwindow.h"
#include "initdb.h"

#include <QApplication>
#include <QtSql>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    initDb();
    w.show();
    return a.exec();
}
