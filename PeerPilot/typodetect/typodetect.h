#ifndef TYPODETECT_H
#define TYPODETECT_H

#include <QDialog>
#include "../PeerPilotSurveyReader.h"
#include <QtSql>

namespace Ui {
class typodetect;
}

class typodetect : public QDialog
{
    Q_OBJECT

public:
    explicit typodetect(QWidget *parent, ResponseList responsesInput, std::vector<std::string> titlesInput, QString classNameInput, std::vector<std::pair<std::string, std::string>> matchListInput);
    ~typodetect();
    ResponseList responses;
    std::vector<std::string> titles;
    QStringList mismatches;
    QStringList students;
    std::vector<std::pair<std::string, std::string>> matchList;
    QString className;

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
