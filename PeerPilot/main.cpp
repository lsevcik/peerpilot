#include "mainwindow/mainwindow.h"
#include "mainwindow.h"
#include "initdb.h"

#include <QApplication>
#include <QtSql/qsqldatabase.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    initDb();
    w.show();
    return a.exec();
}
