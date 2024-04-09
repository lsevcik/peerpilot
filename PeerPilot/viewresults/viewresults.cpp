#include "viewresults.h"
#include "ui_viewresults.h"
#include "../PeerPilotSurveyReader.h"
#include <QFileDialog>
#include <QtSql>
#include <QMessageBox>


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

        // Add the response to the ResponseList
        responses.addResponse(response);
    }

    file.close();


    QMessageBox::information(this, "PeerPilot", "CSV file imported successfully");

    return 0;
}
