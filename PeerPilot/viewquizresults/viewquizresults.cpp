#include "viewquizresults.h"
#include "ui_viewquizresults.h"
#include <QFileDialog>
#include "../PeerPilotSurveyReader.h"
#include <QDialog>
#include <QWidget>
#include <QtSql>
#include <QMessageBox>
#include <QInputDialog>
viewquizresults::viewquizresults(QWidget *parent, ResponseList responsesInput, std::vector<std::string> titlesInput, QString classNameInput)
    : QWidget(parent)
    , ui(new Ui::viewquizresults)
{
    ui->setupUi(this);
    connect(ui->resultListView, &QListView::clicked, this, &viewquizresults::on_resultListView_clicked);
    connect(ui->questionComboBox, &QComboBox::currentIndexChanged, this, &viewquizresults::on_questionComboBox_currentIndexChanged);

    //QMessageBox::information(this, "PeerPilot", className);
    // Call the getData function with the file path
    responses = responsesInput;

    className = classNameInput;

    // Get question titles from data
    titles = titlesInput;

    // Load questions into combobox
    QStringList QTitles;
    for (const auto& title : titles) {
        QTitles.append(QString::fromStdString(title));
    }

    ui->questionComboBox->addItems(QTitles);

    // Get students from database
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

    // Force update
    on_questionComboBox_currentIndexChanged(ui->questionComboBox->currentIndex());

}

void viewquizresults::on_questionComboBox_currentIndexChanged(int index){
    ui->gradeLineEdit->clear();
    if(!ui->resultListView->currentIndex().isValid()){
        return;
    }
    QString selectedStudent = ui->resultListView->selectionModel()->currentIndex().data().toString();

    std::vector<PeerReview> peerReviews = responses.getPeerReviewsByPeerName(reformatName(selectedStudent.toStdString()));

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

    //set grade
    for(auto& question : gradedQuestions){
        if(question.first == ui->questionComboBox->currentText().toStdString()){
            int grade = 0;
            for(auto& review : peerReviews){
                std::string sInput = review.getAnswers()[index];
                std::string sOutput = std::regex_replace(sInput, std::regex(R"([\D])"), "");
                grade += std::stoi(sOutput);
            }
            grade /= peerReviews.size();
            ui->gradeLineEdit->insert(QString::number(grade) + "/" + QString::number(question.second));
        }
    }

}

void viewquizresults::on_markGradePushButton_clicked(){

    int grade = QInputDialog::getInt(this,"Mark question as graded?","Enter the maximum possible score for this question:\n" + ui->questionComboBox->currentText());

    gradedQuestions.push_back(std::make_pair(ui->questionComboBox->currentText().toStdString(), grade));

    if(!ui->resultListView->currentIndex().isValid()){
        return;
    }

    QString selectedStudent = ui->resultListView->selectionModel()->currentIndex().data().toString();

    std::vector<PeerReview> peerReviews = responses.getPeerReviewsByPeerName(reformatName(selectedStudent.toStdString()));

    // Force update
    on_questionComboBox_currentIndexChanged(ui->questionComboBox->currentIndex());
}

void viewquizresults::on_exportGradesPushButton_clicked(){
    if(!ui->resultListView->currentIndex().isValid()){
        return;
    }

    int maxPoints = QInputDialog::getInt(this,"Exporting gradesheet","Enter the maximum points possible for this assignment:\n");

    std::string fileContents = "";

    //GET ALL STUDENT DATA, TABULATE GRADES FROM ALL GRADED QUESTIONS


    /*
    for (int i = 0; i < peerReviews[0].getAnswers().size(); i++)
    {
        fileContents += titles[i] + "\n";
        for (auto& review : peerReviews) {
            fileContents += review.getAnswers()[i] + "\n";
        }
        fileContents += "\n";
    }*/

    std::string suggestedFileName = className.toStdString()+".csv";

    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), suggestedFileName.c_str());
    QFile f( filename );
    f.open( QIODevice::WriteOnly );
    // store data in f
    f.QIODevice::write(fileContents.c_str());
    f.close();
}

void viewquizresults::on_exportAllPushButton_clicked(){
    if(!ui->resultListView->currentIndex().isValid()){
        return;
    }

    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Output Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);

    for(auto student : students){
        std::vector<PeerReview> peerReviews = responses.getPeerReviewsByPeerName(reformatName(student.toStdString()));

        std::string fileContents = "";

        for (int i = 0; i < peerReviews[0].getAnswers().size(); i++)
        {
            fileContents += titles[i] + "\n";
            for (auto& review : peerReviews) {
                fileContents += review.getAnswers()[i] + "\n";
            }
            fileContents += "\n";
        }

        QString fileName = dir+"/"+student+".txt";

        QFile f( fileName );
        f.open( QIODevice::WriteOnly );
        // store data in f
        f.QIODevice::write(fileContents.c_str());
        f.close();
    }
}


