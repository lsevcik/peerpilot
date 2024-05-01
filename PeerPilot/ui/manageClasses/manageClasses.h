#ifndef VIEWCLASSES_H
#define VIEWCLASSES_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class manageClasses;
}

class manageClasses : public QDialog
{
    Q_OBJECT

public:
    explicit manageClasses(QWidget *parent = nullptr);
    ~manageClasses();

private slots:
    void on_createButton_clicked();
    void on_updateButton_clicked();
    void on_deleteButton_clicked();

private:
    Ui::manageClasses *ui;
    QSqlTableModel classListModel;
};

#endif // VIEWCLASSES_H
