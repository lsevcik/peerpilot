#include "viewclasses.h"
#include "ui_viewclasses.h"

viewclasses::viewclasses(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::viewclasses)
{
    ui->setupUi(this);
}

viewclasses::~viewclasses()
{
    delete ui;
}
