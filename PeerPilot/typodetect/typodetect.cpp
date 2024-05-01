#include "typodetect.h"
#include "ui_typodetect.h"
#include <QWidget>
#include <QMessageBox>
#include "../viewquizresults/viewquizresults.h"


typodetect::typodetect(QWidget *parent, ResponseList responsesInput, std::vector<std::string> titlesInput, QString classNameInput, std::vector<std::pair<std::string, std::string>> matchListInput)
    : QDialog(parent)
    , ui(new Ui::typodetect)
{
    ui->setupUi(this);
    connect(ui->unknownNamesListView, &QListView::clicked, this, &typodetect::on_unknownNamesListView_clicked);
    connect(ui->classNamesListView, &QListView::clicked, this, &typodetect::on_classNamesListView_clicked);

    //Save inputs
    responses = responsesInput;
    titles = titlesInput;
    className = classNameInput;
    matchList = matchListInput;


    // Load mismatches into list
    for (const auto& pair : matchList) {
        mismatches.append(QString::fromStdString(pair.first));
    }

    QStringListModel *model = new QStringListModel(mismatches);

    // Set the model to the resultListView
    ui->unknownNamesListView->setModel(model);

    // Load students into list
    // Get students from database
    q.prepare("SELECT students.name FROM students JOIN classes ON students.class_id = classes.id WHERE title=?");
    q.addBindValue(className);
    q.exec();

    while (q.next()) {
        students << q.value(0).toString();
    }

    QStringListModel *modelStudents = new QStringListModel(students);

    // Set the model to the resultListView
    ui->classNamesListView->setModel(modelStudents);

}

typodetect::~typodetect()
{
    delete ui;
}



void typodetect::on_setNamePushButton_clicked() {
    std::string oldName = ui->unknownNamesListView->selectionModel()->currentIndex().data().toString().toStdString();
    std::string newName = ui->classNamesListView->selectionModel()->currentIndex().data().toString().toStdString();

    if(oldName == "" || newName == ""){
        return;
    }

    std::string message = "Confirm the following change?\n" + oldName + " -> " + newName;

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Set name", QString::fromUtf8(message.c_str()), QMessageBox::Yes|QMessageBox::No);

    if(reply == QMessageBox::Yes){
        // Replace the name
        responses.replaceName(oldName, newName);
        // Remove from list
        ui->unknownNamesListView->model()->removeRow(ui->unknownNamesListView->selectionModel()->selectedIndexes().at(0).row());
    }
}

void typodetect::on_confirmPushButton_clicked() {
    if(ui->unknownNamesListView->model()->rowCount() != 0){
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Unset Names", "There are still unknown names. Are you sure you want to continue?\nAll reviews for unknown names will be lost.", QMessageBox::Yes|QMessageBox::No);

        if(reply == QMessageBox::No){
            return;
        }
    }

    // Open results view
    auto widget = new viewquizresults(this->parentWidget(), responses, titles, className);
    widget->show();
    this->close();
}

void typodetect::on_unknownNamesListView_clicked(const QModelIndex &index) {

}

void typodetect::on_classNamesListView_clicked(const QModelIndex &index) {

}

