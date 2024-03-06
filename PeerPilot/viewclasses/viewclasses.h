#ifndef VIEWCLASSES_H
#define VIEWCLASSES_H

#include <QDialog>

namespace Ui {
class viewclasses;
}

class viewclasses : public QDialog
{
    Q_OBJECT

public:
    explicit viewclasses(QWidget *parent = nullptr);
    ~viewclasses();

private:
    Ui::viewclasses *ui;
};

#endif // VIEWCLASSES_H
