#ifndef VIEWCLASSES_H
#define VIEWCLASSES_H

#include <QWidget>

namespace Ui {
class viewclasses;
}

class viewclasses : public QWidget
{
    Q_OBJECT

public:
    explicit viewclasses(QWidget *parent = nullptr);
    ~viewclasses();

private:
    Ui::viewclasses *ui;
};

#endif // VIEWCLASSES_H
