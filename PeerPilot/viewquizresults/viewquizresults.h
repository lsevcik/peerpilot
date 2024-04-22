#ifndef VIEWQUIZRESULTS_H
#define VIEWQUIZRESULTS_H

#include <QWidget>
#include "../PeerPilotSurveyReader.h"
#include <QtSql>

namespace Ui {
class viewquizresults;
}

class viewquizresults : public QWidget
{
    Q_OBJECT

public:
    explicit viewquizresults(QWidget *parent, ResponseList responsesInput, std::vector<std::string> titlesInput, QString className);
    ~viewquizresults();
    ResponseList responses;
    std::vector<std::string> titles;
    QStringList students;

private slots:
    //void on_resultListView_indexesMoved(const QModelIndexList &indexes);
    void on_resultListView_clicked(const QModelIndex &index);
    void on_questionComboBox_currentIndexChanged(int index);
    void on_exportSinglePushButton_clicked();
    void on_exportAllPushButton_clicked();

private:
    Ui::viewquizresults *ui;
    QSqlQuery q;
    QSqlTableModel studentListModel;

};

#endif // VIEWQUIZRESULTS_H
