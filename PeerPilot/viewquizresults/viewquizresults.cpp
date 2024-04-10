#include "viewquizresults.h"
#include "ui_viewquizresults.h"
#include <QFileDialog>
#include "../PeerPilotSurveyReader.h"
#include <QDialog>
#include <QWidget>
#include <QtSql>
#include <QMessageBox>
viewquizresults::viewquizresults(QWidget *parent, std::string filePath, QString className)
    : QWidget(parent)
    , ui(new Ui::viewquizresults)
{
    ui->setupUi(this);
    connect(ui->resultListView, &QListView::clicked, this, &viewquizresults::on_resultListView_clicked);
    connect(ui->questionComboBox, &QComboBox::currentIndexChanged, this, &viewquizresults::on_questionComboBox_currentIndexChanged);

    //QMessageBox::information(this, "PeerPilot", className);
    // Call the getData function with the file path
    responses = getData(filePath);

    // Get question titles from data
    titles = getQuestionTitles(filePath);

    // Load questions into combobox
    QStringList QTitles;
    for (const auto& title : titles) {
        QTitles.append(QString::fromStdString(title));
    }

    ui->questionComboBox->addItems(QTitles);

    // Get students from database
    QStringList students;

    q.prepare("SELECT students.name FROM students JOIN classes ON students.class_id = classes.id WHERE title=?");
    q.addBindValue(className);
    q.exec();

    while (q.next()) {
        students << q.value(0).toString();
    }

    QStringListModel *model = new QStringListModel(students);

    // Set the model to the resultListView
    ui->resultListView->setModel(model);


    /*
    for (const auto& response : responses.getResponses()) {
        // Generate string representation of each response
        std::string responseString = "Response Name: " + response.getName() + "\n";
        responseString += "Response ID: " + std::to_string(response.getId()) + "\n";

        // Iterate over peer reviews for this response
        for (const auto& peerReview : response.getPeerReviews()) {
            responseString += "- Peer Name: " + peerReview.getPeerName() + "\n";
            responseString += "  Origin ID: " + std::to_string(peerReview.getOriginId()) + "\n";

            // Iterate over answers for this peer review
            for (const auto& answer : peerReview.getAnswers()) {
                responseString += "  - " + answer + "\n";
            }
        }



        // Append the response string to the QTextEdit widget
        ui->answerTextEdit->append(QString::fromStdString(responseString));
    }
    ui->answerTextEdit->append(QString::fromStdString("test"));
*/

}

viewquizresults::~viewquizresults()
{
    delete ui;
}

void viewquizresults::on_resultListView_clicked(const QModelIndex &index){

    // Force updare
    on_questionComboBox_currentIndexChanged(ui->questionComboBox->currentIndex());

}

void viewquizresults::on_questionComboBox_currentIndexChanged(int index){
    if(!ui->resultListView->currentIndex().isValid()){
        return;
    }
    //if(!ui->resultListView->selectionModel()->currentIndex().isValid()){
    //    return;
    //}
    QString selectedStudent = ui->resultListView->selectionModel()->currentIndex().data().toString();

    std::vector<PeerReview> peerReviews = responses.getPeerReviewsByPeerName(selectedStudent.toStdString());

    std::string responseString = "";

    if(peerReviews.empty()){
        ui->answerTextEdit->clear();
        return;
    }
    for (auto& review : peerReviews) {
        responseString += review.getAnswers()[index] + "\n\n";
    }
    ui->answerTextEdit->clear();
    ui->answerTextEdit->append(QString::fromStdString(responseString));

}
