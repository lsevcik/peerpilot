#include "mainwindow/mainwindow.h"
#include "initdb.h"

#include <QApplication>
#include <QMessageBox>
#include <QtSql>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    try {
        initDb();
    } catch (QSqlError& e) {
        if (QMessageBox::critical(nullptr, "Database error", e.text(), QMessageBox::Abort, QMessageBox::Ignore) != QMessageBox::Ignore)
            return 1;
    }
    w.show();
    return a.exec();
}
