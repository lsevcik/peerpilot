#include "viewclasses.h"
#include "ui_viewclasses.h"
#include <QtSql>

viewclasses::viewclasses(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::viewclasses)
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

viewclasses::~viewclasses()
{
    delete ui;
}
