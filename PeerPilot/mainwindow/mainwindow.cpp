#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../ui/manageClasses/manageClasses.h"

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

void MainWindow::on_actionExit_triggered() {
    this->close();
}

void MainWindow::on_actionManage_Classes_triggered() {
    auto dialog = new manageClasses(this);
    dialog->exec();
}