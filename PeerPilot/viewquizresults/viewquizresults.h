#ifndef VIEWQUIZRESULTS_H
#define VIEWQUIZRESULTS_H

#include <QDialog>
#include "../PeerPilotSurveyReader.h"
#include <QtSql>

namespace Ui {
class viewquizresults;
}

class viewquizresults : public QDialog
{
    Q_OBJECT

public:
    explicit viewquizresults(QWidget *parent, ResponseList responsesInput, std::vector<std::string> titlesInput, QString className);
    ~viewquizresults();
    ResponseList responses;
    std::vector<std::string> titles;
    QStringList students;
    // question index, question max points
    std::vector<std::pair<int, int>> gradedQuestions;
    QString className;

private slots:
    //void on_resultListView_indexesMoved(const QModelIndexList &indexes);
    void on_resultListView_clicked(const QModelIndex &index);
    void on_questionComboBox_currentIndexChanged(int index);
    void on_exportGradesPushButton_clicked();
    void on_exportAllPushButton_clicked();
    void on_markGradePushButton_clicked();
    void on_backPushButton_clicked();

private:
    Ui::viewquizresults *ui;
    QSqlQuery q;
    QSqlTableModel studentListModel;

};

#endif // VIEWQUIZRESULTS_H
