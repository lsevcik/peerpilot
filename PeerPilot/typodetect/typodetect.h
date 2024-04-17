#ifndef TYPODETECT_H
#define TYPODETECT_H

#include <QWidget>
#include "../PeerPilotSurveyReader.h"
#include <QtSql>

namespace Ui {
class typodetect;
}

class typodetect : public QWidget
{
    Q_OBJECT

public:
    explicit typodetect(QWidget *parent = nullptr, std::string filePath = nullptr, QString className = nullptr);
    ~typodetect();
    ResponseList responses;
    std::vector<std::string> titles;
    QStringList students;

private slots:
    //void on_resultListView_indexesMoved(const QModelIndexList &indexes);
    void on_unknownNamesListView_clicked(const QModelIndex &index);
    void on_classNamesListView_clicked(const QModelIndex &index);
    void on_setNamePushButton_clicked();
    void on_confirmPushButton_clicked();

private:
    Ui::typodetect *ui;
    QSqlQuery q;
    QSqlTableModel studentListModel;

};

#endif // TYPODETECT_H
