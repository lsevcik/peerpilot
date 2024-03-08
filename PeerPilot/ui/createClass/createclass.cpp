#include "createClass.h"
#include "ui_createClass.h"

createClass::createClass(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::createClass) {
    ui->setupUi(this);
    q.exec("INSERT INTO classes (title) VALUES (new_class)");
    classId = q.lastInsertId().toString();

    auto model = new QSqlTableModel;
    model->setTable("students");
    model->select();
    model->setFilter(QString("class=") + classId);
    ui->studentTableView->setModel(model);
    ui->studentTableView->hideColumn(1);
    //studentListModel.setQuery(QString("SELECT * FROM students WHERE class=") + classId);
    //ui->studentListView->setModel(studentListModel);
    q.prepare(QString("INSERT INTO students (name, class) VALUES (?, ") + classId + ")");
}

createClass::~createClass() {
    delete ui;
}

void createClass::on_manualEntryNamePushButton_clicked() {
    auto name = ui->studentNameLineEdit->text();
    q.addBindValue(name);
    if (!q.exec())
        qDebug() << q.lastError();
}

void createClass::on_savePushButton_clicked() {
    q.prepare(QString("UPDATE classes SET title=? WHERE id=") + classId);
    q.addBindValue(ui->classNameLineEdit->text());
    q.exec();
    this->close();
}