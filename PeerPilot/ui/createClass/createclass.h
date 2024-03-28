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
    explicit createClass(QWidget *parent = nullptr, QString className = "");
    ~createClass();

private slots:
    void on_manualEntryNamePushButton_clicked();
    void on_savePushButton_clicked();
    int on_importPushButton_clicked();

private:
    Ui::createClass *ui;
    QSqlQuery q;
    QSqlTableModel studentListModel;
    int classId;
};

#endif // CREATECLASS_H
