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

//Button that insert data to DB
void MainWindow::on_pushButton_InsertData_clicked()
{
    //Prevent simultaneous access to DB
    QSqlDatabase::database().transaction();
    //create a sqlquery instance that will let us query the DB with CRUD statements
    QSqlQuery q;

    q.prepare("INSERT INTO classes(sisid, title) VALUES(:id, :title)");

    //for the values we are adding to DB, give their name then origin
    //                              name        ,        origin
    q.bindValue(":id", ui->lineEdit->text().toInt());
    q.bindValue(":title", ui->lineEdit_2->text());

    if (!q.exec())
        qDebug() << q.lastError();
    QSqlDatabase::database().commit();
}


void MainWindow::on_pushButton_InsertData_2_clicked()
{
    //Prevent simultaneous access to DB
    QSqlDatabase::database().transaction();
    //create a sqlquery instance that will let us query the DB with CRUD statements
    QSqlQuery QueryLoadData;

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
}

