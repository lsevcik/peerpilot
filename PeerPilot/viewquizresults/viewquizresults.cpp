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

    //QMessageBox::information(this, "PeerPilot", className);
    // Call the getData function with the file path
    responses = getData(filePath);

    // Get question titles from data
    titles = getQuestionTitles(filePath);

    // Get students from database
    QStringList students;

    q.prepare("SELECT students.name FROM students JOIN classes ON students.class_id = classes.id WHERE title=?");
    q.addBindValue("Class 1");
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
    QString selectedItem = index.data().toString();
    QMessageBox::information(this, "PeerPilot", selectedItem);
    //QMessageBox::information(this, "PeerPilot", "className");

    // Get selected class
    std::string name = selectedItem.toStdString();

    std::vector<PeerReview> peerReviews = responses.getPeerReviewsByPeerName(name);

    std::string responseString = "";

    for (int i = 0; i < peerReviews[0].getAnswers().size(); i++)
    {
        responseString += titles[i] + "\n";
        for (auto& review : peerReviews) {
            responseString += review.getAnswers()[i] + "\n";
        }
    }

    ui->answerTextEdit->append(QString::fromStdString(responseString));
    //ui->answerTextEdit->append(QString::fromStdString("test"));
}
