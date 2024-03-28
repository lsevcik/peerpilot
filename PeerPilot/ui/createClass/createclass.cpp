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

int createClass::on_importPushButton_clicked() {
    QFileDialog dialog(this);
    auto fileName = QFileDialog::getOpenFileName(this,
        tr("Open Roster"), QDir::homePath(), tr("Spreadsheets (*.xls *.xlsx *.csv)"));

    QFile file(fileName);

    if (!file.exists())
        return QMessageBox::warning(this, "PeerPilot", "File does not exist");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QMessageBox::warning(this, "PeerPilot", "Error opening file");

    QTextStream s(&file);

    if (s.atEnd() || s.read(56) != "Student,ID,SIS User ID,SIS Login ID,Root Account,Section")
        return QMessageBox::warning(this, "PeerPilot", "Incorrect format");
    
    // Ignore the rest of the headers
    s.readLine();
    s.readLine();

    while (!s.atEnd()) {
        auto components = s.readLine().split(',');
        // TODO parse csv correctly

        auto classField = QSqlField("class_id", QVariant::Int, "students");
        classField.setValue(classId);

        auto nameField = QSqlField("name", QVariant::String, "students");
        nameField.setValue(components[1] + " " + components[0]);
        // TODO: Strip quotes

        auto canvasIdField = QSqlField("canvas_id", QVariant::Int, "students");
        canvasIdField.setValue(components[2]);

        auto sisIdField = QSqlField("sis_id", QVariant::Int, "students");
        sisIdField.setValue(components[3]);

        auto sisUsernameField = QSqlField("sis_username", QVariant::String, "students");
        sisUsernameField.setValue(components[4]);

        auto sectionField = QSqlField("section", QVariant::String, "students");
        sectionField.setValue(components[6]);

        auto studentRecord = QSqlRecord();
        studentRecord.append(classField);
        studentRecord.append(nameField);
        studentRecord.append(canvasIdField);
        studentRecord.append(sisIdField);
        studentRecord.append(sisUsernameField);
        studentRecord.append(sectionField);
        studentListModel.insertRecord(-1, studentRecord);
    }
}