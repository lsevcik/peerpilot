#ifndef VIEWRESULTS_H
#define VIEWRESULTS_H

#include <QDialog>

namespace Ui {
class viewresults;
}

class viewresults : public QDialog
{
    Q_OBJECT

public:
    explicit viewresults(QWidget *parent = nullptr);
    ~viewresults();

private:
    Ui::viewresults *ui;
};

#endif // VIEWRESULTS_H
