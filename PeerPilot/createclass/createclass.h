#ifndef CREATECLASS_H
#define CREATECLASS_H

#include <QWidget>

namespace Ui {
class createclass;
}

class createclass : public QWidget
{
    Q_OBJECT

public:
    explicit createclass(QWidget *parent = nullptr);
    ~createclass();

private:
    Ui::createclass *ui;
};

#endif // CREATECLASS_H
