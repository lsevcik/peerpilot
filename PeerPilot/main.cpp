#include "mainwindow/mainwindow.h"
#include "initdb.h"

#include <QApplication>
#include <QtSql>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << initDb();
    MainWindow w;
    w.show();
    return a.exec();
}
