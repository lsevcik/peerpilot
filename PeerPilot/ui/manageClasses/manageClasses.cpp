#include "manageClasses.h"
#include "ui_manageClasses.h"
#include "../createClass/createClass.h"
#include <QtSql>

manageClasses::manageClasses(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::manageClasses)
{
    ui->setupUi(this);
    QSqlQueryModel* model = new QSqlQueryModel;
    model->setQuery("SELECT title FROM classes");
    ui->classListView->setModel(model);
    ui->classListView->show();
}

manageClasses::~manageClasses()
{
    delete ui;
}

void manageClasses::on_createButton_clicked() {
    auto dialog = new createClass(this);
    dialog->exec();
}