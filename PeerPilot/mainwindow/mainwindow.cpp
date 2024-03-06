#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getDBconnection(){
    db = QSqlDatabase::database("QSQLITE");
    qDebug() << db.drivers();
    if(db.isOpen())
        fillTable();
    else
        qDebug() << db.lastError();
}

void MainWindow::fillTable(){
    //Open connection to DB
    db.open();
    //Prevent simultaneous access to DB
    db.database().transaction();
    //create a sqlquery instance that will let us query the DB with CRUD statements
    QSqlQuery q(db);

    //Here we write the query
    q.prepare("SELECT * FROM classes");
    if(q.exec()){
        while(q.next()){
            ui->listWidget_showClasses->addItem(q.value("id").toString()
                                                + "\t"
                                                + q.value("title").toString());
        }
    }

    //commit request to DB
    db.database().commit();
    //close connection
    db.close();
}
