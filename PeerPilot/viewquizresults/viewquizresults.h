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
    explicit viewquizresults(QWidget *parent = nullptr, std::string filePath = nullptr, QString className = nullptr);
    ~viewquizresults();
    ResponseList responses;
    std::vector<std::string> titles;

private slots:
    //void on_resultListView_indexesMoved(const QModelIndexList &indexes);
    void on_resultListView_clicked(const QModelIndex &index);

private:
    Ui::viewquizresults *ui;
    QSqlQuery q;
    QSqlTableModel studentListModel;

};

#endif // VIEWQUIZRESULTS_H
