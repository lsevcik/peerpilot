#include "viewresults.h"
#include "ui_viewresults.h"
#include "../PeerPilotSurveyReader.h"
#include "../viewquizresults/viewquizresults.h"
#include <QFileDialog>
#include <QtSql>
#include <QMessageBox>

#include <iostream>


viewresults::viewresults(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::viewresults)
{
    ui->setupUi(this);
    QSqlQueryModel* model = new QSqlQueryModel;
    model->setQuery("SELECT title FROM classes");
    model->setHeaderData(0, Qt::Horizontal, tr("Title"));
    qDebug() << model->record(0).value("title");
    ui->classListView->setModel(model);
    ui->classListView->setModelColumn(0);
    ui->classListView->show();

    connect(ui->importQuizResultsPushButton, &QPushButton::clicked, this, &viewresults::on_importQuizPushButton_clicked);

}

viewresults::~viewresults()
{
    delete ui;
}

int viewresults::on_importQuizPushButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open CSV File"), QDir::homePath(), tr("CSV Files (*.csv)"));
    QFile file(fileName);

    // Check if the file exists
    if (!file.exists())
        return QMessageBox::warning(this, "PeerPilot", "File does not exist");

    // Open the file
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QMessageBox::warning(this, "PeerPilot", "Error opening file");
    // Convert QString to std::string
    std::string filePath = fileName.toStdString();

    // Get selected class
    QString className = ui->classListView->selectionModel()->currentIndex().data().toString();

    // Open results view
    auto widget = new viewquizresults(this, filePath, className);
    widget->show();

    /*
    // Read the CSV file using QTextStream
    QTextStream s(&file);

    QString headersLine = s.readLine();

    ResponseList responses;

    // Read each line of the CSV file
    while (!s.atEnd()) {
        QString line = s.readLine();
        QStringList components = line.split(',');

        // Process the components and create Response objects
        QString name = components[0];
        int id = components[1].toInt();

        Response response(name.toStdString(), id);

        for (int i = 2; i < components.size(); ++i) {
            PeerReview peerReview(id, "Peer " + QString::number(i - 1).toStdString());
            peerReview.addAnswer(components[i].toStdString());
            response.addPeerReview(peerReview);
        }
        std::cout << "TEST";
        response.print();
        // Add the response to the ResponseList
        responses.addResponse(response);
    }
    */
    file.close();
    /*
    // Insert data into the database
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    // Iterate over each response and insert data into database
    for (const Response& response : responses.getResponses()) {
        // Insert response data into classes table
        query.prepare("INSERT INTO classes (title) VALUES (:title)");
        query.bindValue(":title", QString::fromStdString(response.getName()));
        if (!query.exec()) {
            qDebug() << "Error inserting data into classes table:" << query.lastError().text();
            // Handle error
        }

        // Retrieve the last inserted class ID
        QString classId = query.lastInsertId().toString();

        // Insert student data into students table
        for (const PeerReview& peerReview : response.getPeerReviews()) {
            query.prepare("INSERT INTO students (name, class_id, canvas_id, sis_id, sis_username, section) "
                          "VALUES (:name, :class_id, :canvas_id, :sis_id, :sis_username, :section)");
            query.bindValue(":name", QString::fromStdString(peerReview.getPeerName()));
            query.bindValue(":class_id", classId);
            // Add other bindings as needed (canvas_id, sis_id, sis_username, section)
            if (!query.exec()) {
                qDebug() << "Error inserting data into students table:" << query.lastError().text();
                // Handle error
            }
        }
    }
    */
    //QMessageBox::information(this, "PeerPilot", "CSV file imported successfully");

    return 0;
}
