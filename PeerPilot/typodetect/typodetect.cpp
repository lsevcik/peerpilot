#include "typodetect.h"
#include "ui_typodetect.h"
#include <QWidget>

typodetect::typodetect(QWidget *parent, std::string filePath, QString className)
    : QWidget(parent)
    , ui(new Ui::typodetect)
{
    ui->setupUi(this);
    connect(ui->unknownNamesListView, &QListView::clicked, this, &typodetect::on_unknownNamesListView_clicked);
    connect(ui->classNamesListView, &QListView::clicked, this, &typodetect::on_classNamesListView_clicked);


}

typodetect::~typodetect()
{
    delete ui;
}



void typodetect::on_setNamePushButton_clicked() {

}

void typodetect::on_confirmPushButton_clicked() {

}

void typodetect::on_unknownNamesListView_clicked(const QModelIndex &index) {

}

void typodetect::on_classNamesListView_clicked(const QModelIndex &index) {

}

