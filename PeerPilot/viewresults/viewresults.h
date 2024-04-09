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

private slots:
    int on_importQuizPushButton_clicked();


private:
    Ui::viewresults *ui;
};

#endif // VIEWRESULTS_H
