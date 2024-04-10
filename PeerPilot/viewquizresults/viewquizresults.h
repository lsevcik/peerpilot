#ifndef VIEWQUIZRESULTS_H
#define VIEWQUIZRESULTS_H

#include <QWidget>
#include "../PeerPilotSurveyReader.h"

namespace Ui {
class viewquizresults;
}

class viewquizresults : public QWidget
{
    Q_OBJECT

public:
    explicit viewquizresults(QWidget *parent = nullptr, const ResponseList& responses = ResponseList());
    ~viewquizresults();

private:
    Ui::viewquizresults *ui;
};

#endif // VIEWQUIZRESULTS_H
