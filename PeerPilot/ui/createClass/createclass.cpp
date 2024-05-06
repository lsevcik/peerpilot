#include "createClass.h"
#include "ui_createClass.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QMetaType>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QDebug>
#include <iostream>

createClass::createClass(QWidget* parent, QString className, bool create)
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
        if (!create) {
            ui->groupBox->setTitle("Manage Class");
            this->setWindowTitle("Manage Class");
        }
        q.next();
        classId = q.value(0).toInt();
    }

    ui->classNameLineEdit->setText(className);

    studentListModel.setTable("students");
    studentListModel.select();
    studentListModel.setFilter(QString("class_id=") + QString::number(classId));
    ui->studentTableView->setModel(&studentListModel);
    for (auto& i : {0, 2})
        ui->studentTableView->hideColumn(i);

    ui->studentTableView->model()->setHeaderData(1, Qt::Horizontal, "Name");
    ui->studentTableView->model()->setHeaderData(3, Qt::Horizontal, "SIS ID");
    ui->studentTableView->model()->setHeaderData(4, Qt::Horizontal, "Canvas ID");
    ui->studentTableView->model()->setHeaderData(5, Qt::Horizontal, "SIS Username");
    ui->studentTableView->model()->setHeaderData(6, Qt::Horizontal, "Section");

    auto importFileAction = new QAction(this);
    importFileAction->setText("Import from CSV File");
    connect(importFileAction, &QAction::triggered, this, &createClass::importFromFile);

    auto importApiAction = new QAction(this);
    importApiAction->setText("Import from Canvas API");
    connect(importApiAction, &QAction::triggered, this, &createClass::importFromAPI);

    auto menu = new QMenu(this);
    menu->addActions({ importFileAction, importApiAction });
    
    ui->importPushButton->setMenu(menu);
}

createClass::~createClass() {
    q.prepare(QString("UPDATE classes SET title=? WHERE id=") + QString::number(classId));
    q.addBindValue(ui->classNameLineEdit->text());
    q.exec();
    delete ui;
}

void createClass::closeEvent(QCloseEvent* e) {
    q.prepare(QString("UPDATE classes SET title=? WHERE id=") + QString::number(classId));
    q.addBindValue(ui->classNameLineEdit->text());
    q.exec();
}

void createClass::on_manualEntryNamePushButton_clicked() {
    auto name = ui->studentNameLineEdit->text();
    ui->studentNameLineEdit->clear();
    auto nameField = QSqlField("name", QMetaType::fromType<std::string>(), "students");
    nameField.setValue(name);
    auto classField = QSqlField("class_id", QMetaType::fromType<int>(), "students");
    classField.setValue(classId);
    auto studentRecord = QSqlRecord();
    studentRecord.append(nameField);
    studentRecord.append(classField);
    studentListModel.insertRecord(-1, studentRecord);
}

void createClass::on_savePushButton_clicked() {
    this->close();
}

int createClass::importFromFile() {
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

        auto classField = QSqlField("class_id", QMetaType::fromType<int>(), "students");
        classField.setValue(classId);

        auto nameField = QSqlField("name", QMetaType::fromType<std::string>(), "students");
        nameField.setValue(components[0].removeAt(0) + ',' + components[1].removeAt(components[1].size()-1));

        auto canvasIdField = QSqlField("canvas_id", QMetaType::fromType<int>(), "students");
        canvasIdField.setValue(components[2]);

        auto sisIdField = QSqlField("sis_id", QMetaType::fromType<int>(), "students");
        sisIdField.setValue(components[3]);

        auto sisUsernameField = QSqlField("sis_username", QMetaType::fromType<std::string>(), "students");
        sisUsernameField.setValue(components[4]);

        auto sectionField = QSqlField("section", QMetaType::fromType<std::string>(), "students");
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
    return 0;
}


int createClass::chooseClassForAPI(QNetworkReply* reply) {
    if (reply->error() != QNetworkReply::NoError)
        return QMessageBox::critical(this, "PeerPilot", "Error getting class list from API");

    auto classJsonDoc = QJsonDocument::fromJson(reply->readAll());
    if (classJsonDoc.isNull())
        return QMessageBox::critical(this, "PeerPilot", "Error getting class list from API");

    auto classJsonObj = classJsonDoc.object();
    auto data = classJsonObj["data"].toObject();
    qDebug() << data;
    auto allCourses = data["allCourses"].toObject();
    qDebug() << allCourses;
    for (auto c : allCourses)
        qDebug() << c;
}

int createClass::importFromAPI() {
    auto canvasUrl = "https://canvas.instructure.com/api/graphql";
    auto canvasAuthToken = "7~gVXNWfNOe3bwfJBjilJSo2h7BC4Zw1yccq8P6PLRwT4cFeso8kKnWFLQ2JX9rnK7";
    auto canvasQUrl = QUrl(canvasUrl);
    auto canvasRequest = QNetworkRequest(canvasQUrl);
    auto requestBody = QUrlQuery();
    canvasRequest.setRawHeader("Authorization", QString("Bearer ").append(canvasAuthToken).toLocal8Bit());
    requestBody.addQueryItem("query", "{ allCourses { id, sisId, name } }");
    connect(&netMan, &QNetworkAccessManager::finished, this, &createClass::chooseClassForAPI);
    auto res = netMan.post(canvasRequest, requestBody.toString().toLocal8Bit());
    return 0;
}
