#ifndef VIEWCLASSES_H
#define VIEWCLASSES_H

#include <QDialog>

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

private:
    Ui::manageClasses *ui;
};

#endif // VIEWCLASSES_H
