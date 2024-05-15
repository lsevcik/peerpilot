#include "viewquizresults.h"
#include "ui_viewquizresults.h"
#include <QFileDialog>
#include "../PeerPilotSurveyReader.h"
#include <QDialog>
#include <QWidget>
#include <QtSql>
#include <QMessageBox>
#include <QInputDialog>
#include <limits>

viewquizresults::viewquizresults(QWidget *parent, ResponseList responsesInput, std::vector<std::string> titlesInput, QString classNameInput)
    : QDialog(parent)
    , ui(new Ui::viewquizresults)
{
    ui->setupUi(this);
    connect(ui->resultListView, &QListView::clicked, this, &viewquizresults::on_resultListView_clicked);
    connect(ui->questionComboBox, &QComboBox::currentIndexChanged, this, &viewquizresults::on_questionComboBox_currentIndexChanged);

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

    std::vector<PeerReview> peerReviews = responses.getPeerReviewsByPeerName(selectedStudent.toStdString());

    std::string responseString = "";

    if(peerReviews.empty()){
        ui->answerTextEdit->clear();
        return;
    }
    for (auto& review : peerReviews) {
        responseString += review.getAnswers()[index] + "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    }
    ui->answerTextEdit->clear();
    ui->answerTextEdit->append(QString::fromStdString(responseString));

    //set grade
    for(auto& question : gradedQuestions){
        if(question.first == ui->questionComboBox->currentIndex()){
            int grade = 0;
            bool gradeableAnswerFound = false;
            for(auto& review : peerReviews){
                // Replace all non numeric characters
                std::string points = std::regex_replace(review.getAnswers()[index], std::regex(R"([\D])"), "");
                if(points == ""){
                    // Escape if there's nothing to grade
                    continue;
                }
                grade += std::stoi(points);
                gradeableAnswerFound = true;
            }
            if(!gradeableAnswerFound){
                continue;
            }
            grade /= peerReviews.size();
            ui->gradeLineEdit->insert(QString::number(grade) + "/" + QString::number(question.second));
        }
    }

}

void viewquizresults::on_markGradePushButton_clicked(){

    if(ui->gradeLineEdit->text() != ""){
        return;
    }
    bool ok = false;
    int grade = QInputDialog::getInt(this,"Mark question as graded?",
                                     "Enter the maximum possible score for this question:\n" + ui->questionComboBox->currentText(),
                                     0,0, std::numeric_limits<long>::max(),1, &ok);

    if(!ok){
        return;
    }

    gradedQuestions.push_back(std::make_pair(ui->questionComboBox->currentIndex(), grade));

    if(!ui->resultListView->currentIndex().isValid()){
        return;
    }

    QString selectedStudent = ui->resultListView->selectionModel()->currentIndex().data().toString();

    std::vector<PeerReview> peerReviews = responses.getPeerReviewsByPeerName(selectedStudent.toStdString());

    // Force update
    on_questionComboBox_currentIndexChanged(ui->questionComboBox->currentIndex());
}

void viewquizresults::on_exportGradesPushButton_clicked(){
    if(!ui->resultListView->currentIndex().isValid()){
        return;
    }
    bool ok = false;

    QString assignmentName = QInputDialog::getText(this,"Exporting gradesheet",
                                                   "Enter the name for this assignment.\n"
                                                   "If you have an existing assignment on canvas, please enter it along with the ID in the following format:\n"
                                                   "Assignment Name(ID)",
                                                   QLineEdit::Normal, NULL, &ok);

    if(!ok){
        return;
    }

    int maxPoints = QInputDialog::getInt(this,"Exporting gradesheet",
                                         "Enter the maximum points possible for this assignment:\n",
                                         0,0,std::numeric_limits<long>::max(), 1, &ok);

    if(!ok){
        return;
    }

    std::string fileContents = "Student, ID, SIS User ID, SIS Login ID, Section, " + assignmentName.toStdString() + "\n";

    fileContents += "Points Possible,,,,," + std::to_string(maxPoints) + "\n";

    //GET ALL STUDENT DATA, TABULATE GRADES FROM ALL GRADED QUESTIONS
    // Get data from database
    q.prepare("SELECT students.name, students.canvas_id, students.sis_id, students.sis_username, students.section "
              "FROM students "
              "JOIN classes ON students.class_id = classes.id "
              "WHERE title=?");
    q.addBindValue(className);
    q.exec();

    while (q.next()) {
        fileContents += makeSafeForCSV(q.value(0).toString().toStdString()) + ",";
        fileContents += makeSafeForCSV(q.value(1).toString().toStdString()) + ",";
        fileContents += makeSafeForCSV(q.value(2).toString().toStdString()) + ",";
        fileContents += makeSafeForCSV(q.value(3).toString().toStdString()) + ",";
        fileContents += makeSafeForCSV(q.value(4).toString().toStdString()) + ",";

        //calculate grade
        std::vector<PeerReview> reviews = responses.getPeerReviewsByPeerName(q.value(0).toString().toStdString());

        std::vector<std::pair<int, int>> grade;


        double points = 0.0;
        double possible = 0.0;

        for(auto& gradedQuestion : gradedQuestions){
            int pointTotal = 0;
            bool gradeableAnswerFound = false;
            for(auto& review : reviews){
                std::string points = std::regex_replace(review.getAnswers()[gradedQuestion.first], std::regex(R"([\D])"), "");
                if(points == ""){
                    // Escape if there's nothing to grade
                    continue;
                }
                pointTotal += std::stoi(points);
                gradeableAnswerFound = true;
            }
            if(!gradeableAnswerFound){
                continue;
            }
            points += pointTotal/reviews.size();
            possible += gradedQuestion.second;
        }

        double totalScore = 0;
        if(possible != 0.0){
            //Only add grade if something gradeable was found in all graded questions
            totalScore = (points / possible) * maxPoints;
        }
        fileContents += std::to_string(totalScore) + "\n";
    }

    QString suggestedFileName = assignmentName+".csv";

    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), suggestedFileName);
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
        std::vector<PeerReview> peerReviews = responses.getPeerReviewsByPeerName(student.toStdString());

        std::string fileContents = "";

        if(peerReviews.empty()){
            continue;
        }

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

void viewquizresults::on_backPushButton_clicked(){
    this->close();
}
