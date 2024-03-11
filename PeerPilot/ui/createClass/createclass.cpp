#include "createClass.h"
#include "ui_createClass.h"
#include <QFileDialog>
#include <QMessageBox>

createClass::createClass(QWidget* parent, QString className)
    : QDialog(parent)
    , ui(new Ui::createClass) {
    ui->setupUi(this);
    q.prepare("SELECT id FROM classes WHERE title=?");
    q.addBindValue(className);
    q.exec();
    if (!q.size()) {
        q.prepare("INSERT INTO classes (title) VALUES (?)");
        q.addBindValue(className);
        if (!q.exec())
            qDebug() << q.lastError();
        classId = q.lastInsertId().toInt();
    } else {
        q.next();
        classId = q.value(0).toInt();
    }

    ui->classNameLineEdit->setText(className);

    studentListModel.setTable("students");
    studentListModel.select();
    studentListModel.setFilter(QString("class_id=") + QString::number(classId));
    ui->studentTableView->setModel(&studentListModel);
    for (auto& i : {0, 2, 3, 4})
        ui->studentTableView->hideColumn(i);
}

createClass::~createClass() {
    delete ui;
}

void createClass::on_manualEntryNamePushButton_clicked() {
    auto name = ui->studentNameLineEdit->text();
    ui->studentNameLineEdit->clear();
    auto nameField = QSqlField("name", QVariant::String, "students");
    nameField.setValue(name);
    auto classField = QSqlField("class_id", QVariant::Int, "students");
    classField.setValue(classId);
    auto studentRecord = QSqlRecord();
    studentRecord.append(nameField);
    studentRecord.append(classField);
    studentListModel.insertRecord(-1, studentRecord);
}

void createClass::on_savePushButton_clicked() {
    q.prepare(QString("UPDATE classes SET title=? WHERE id=") + QString::number(classId));
    q.addBindValue(ui->classNameLineEdit->text());
    q.exec();
    this->close();
}

void createClass::on_importPushButton_clicked() {
    QFileDialog dialog(this);
    auto fileName = QFileDialog::getOpenFileName(this,
        tr("Open Roster"), QDir::homePath(), tr("Spreadsheets (*.xls *.xlsx *.csv)"));
    QMessageBox::information(this, "PeerPilot", "Uninplemented :(");
    // TODO: Import contents of class roster
}