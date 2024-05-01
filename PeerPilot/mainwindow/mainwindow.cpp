#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "../ui/manageClasses/manageClasses.h"
#include "../viewresults/viewresults.h"
#include <QMessageBox>

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

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this, "About PeerPilot", "\u00a9 PeerPilot 2024 Some Rights Reserved");
}


void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_viewResultsButton_clicked() {
    auto dialog = new viewresults(this);
    dialog->exec();
}
