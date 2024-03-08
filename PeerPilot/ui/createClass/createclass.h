#ifndef CREATECLASS_H
#define CREATECLASS_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class createClass;
}

class createClass : public QDialog
{
    Q_OBJECT

public:
    explicit createClass(QWidget *parent = nullptr);
    ~createClass();

private slots:
    void on_manualEntryNamePushButton_clicked();
    void on_savePushButton_clicked();

private:
    Ui::createClass *ui;
    QSqlQuery q;
    QSqlQueryModel studentListModel;
    QString classId;
};

#endif // CREATECLASS_H
