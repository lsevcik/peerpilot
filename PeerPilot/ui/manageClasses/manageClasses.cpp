#include "manageClasses.h"
#include "ui_manageClasses.h"
#include "../createClass/createClass.h"
#include <QInputDialog>
#include <QMessageBox>

manageClasses::manageClasses(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::manageClasses)
{
    ui->setupUi(this);
    classListModel.setTable("classes");
    classListModel.select();
    ui->classListView->setModel(&classListModel);
    ui->classListView->setModelColumn(1);
    ui->classListView->show();
}

manageClasses::~manageClasses()
{
    delete ui;
}

void manageClasses::on_createButton_clicked() {
    bool ok;
    QString className = QInputDialog::getText(this, "Create New Class", "Class Name:", QLineEdit::Normal, "", &ok);
    if (!ok)
        return;

    auto nameField = QSqlField("title", QMetaType::fromType<std::string>(), "classes");
    nameField.setValue(className);
    auto classRecord = QSqlRecord();
    classRecord.append(nameField);
    classListModel.insertRecord(-1, classRecord);
    auto dialog = new createClass(this, className, true);
    dialog->exec();
    classListModel.select();
}

void manageClasses::on_updateButton_clicked() {
    QString className = ui->classListView->selectionModel()->currentIndex().data().toString();
    auto dialog = new createClass(this, className);
    dialog->exec();
    classListModel.select();
}

void manageClasses::on_deleteButton_clicked() {
    auto index = ui->classListView->selectionModel()->currentIndex();
    int row = index.row();
    QString className = index.data().toString();
    if (QMessageBox::question(this, "Delete Class", QString("Are you sure you want to delete ") + className, QMessageBox::Ok | QMessageBox::Cancel) != QMessageBox::Ok)
        return;
    classListModel.removeRow(row);
    classListModel.select();
}