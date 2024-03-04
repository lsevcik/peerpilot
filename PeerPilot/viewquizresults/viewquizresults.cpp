#include "viewquizresults.h"
#include "ui_viewquizresults.h"

viewquizresults::viewquizresults(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::viewquizresults)
{
    ui->setupUi(this);
}

viewquizresults::~viewquizresults()
{
    delete ui;
}
