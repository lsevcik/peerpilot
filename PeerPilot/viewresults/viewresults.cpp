#include "viewresults.h"
#include "ui_viewresults.h"
#include <QtSql>

viewresults::viewresults(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::viewresults)
{
    ui->setupUi(this);
    QSqlQueryModel* model = new QSqlQueryModel;
    model->setQuery("SELECT title FROM classes");
    model->setHeaderData(0, Qt::Horizontal, tr("Title"));
    qDebug() << model->record(0).value("title");
    ui->classListView->setModel(model);
    ui->classListView->setModelColumn(0);
    ui->classListView->show();
}

viewresults::~viewresults()
{
    delete ui;
}