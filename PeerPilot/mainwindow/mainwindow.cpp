#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DB_Connection = QSqlDatabase::addDatabase("QSQLITE");
    //specifiy name of DB
    DB_Connection.setDatabaseName(QCoreApplication::applicationDirPath() + "/Database/peerpilotdata.db");
    if(DB_Connection.open()){
        qDebug() << "Database is connected";
    }
    else{
        qDebug() << "Database is not connected";
        qDebug() << "Error: " << DB_Connection.lastError();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Button that insert data to DB
void MainWindow::on_pushButton_InsertData_clicked()
{
    //Open connection to DB
    DB_Connection.open();
    //Prevent simultaneous access to DB
    QSqlDatabase::database().transaction();
    //create a sqlquery instance that will let us query the DB with CRUD statements
    QSqlQuery QueryInsertData(DB_Connection);

    //Here we write the query
    QueryInsertData.prepare("INSERT INTO StudentReviews(Student_ID, Review) VALUES(:val1, :val2)");

    //for the values we are adding to DB, give their name then origin
    //                              name        ,        origin
    QueryInsertData.bindValue(":val1", ui->lineEdit->text().toInt());
    QueryInsertData.bindValue(":val2", ui->lineEdit_2->text());

    //execute query
    QueryInsertData.exec();
    //commit request to DB
    QSqlDatabase::database().commit();
    //close connection
    DB_Connection.close();
}


void MainWindow::on_pushButton_InsertData_2_clicked()
{
    //Open connection to DB
    DB_Connection.open();
    //Prevent simultaneous access to DB
    QSqlDatabase::database().transaction();
    //create a sqlquery instance that will let us query the DB with CRUD statements
    QSqlQuery QueryLoadData(DB_Connection);

    //Here we write the query
    QueryLoadData.prepare("SELECT * FROM StudentReviews");
    int number_of_rows = 10;
    if(QueryLoadData.exec()){
        ui->tableWidget->setRowCount(number_of_rows);
        int row_number = 0;
        while(QueryLoadData.next()){
            ui->tableWidget->setItem(row_number, 0, new QTableWidgetItem(QString(QueryLoadData.value("Student_ID").toString())));
            ui->tableWidget->setItem(row_number, 1, new QTableWidgetItem(QString(QueryLoadData.value("Review").toString())));
            row_number++;
        }
    }

    //commit request to DB
    QSqlDatabase::database().commit();
    //close connection
    DB_Connection.close();
}

