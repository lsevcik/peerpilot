#ifndef VIEWRESULTS_H
#define VIEWRESULTS_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class viewresults;
}

class viewresults : public QDialog
{
    Q_OBJECT

public:
    explicit viewresults(QWidget *parent = nullptr);
    ~viewresults();
    std::vector<std::string> students;


private slots:
    int on_importQuizPushButton_clicked();


private:
    Ui::viewresults *ui;
    QSqlQuery q;
};

#endif // VIEWRESULTS_H
