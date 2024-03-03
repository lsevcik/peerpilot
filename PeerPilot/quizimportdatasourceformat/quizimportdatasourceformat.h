#ifndef QUIZIMPORTDATASOURCEFORMAT_H
#define QUIZIMPORTDATASOURCEFORMAT_H

#include <QWidget>

namespace Ui {
class QuizImportDatasourceFormat;
}

class QuizImportDatasourceFormat : public QWidget
{
    Q_OBJECT

public:
    explicit QuizImportDatasourceFormat(QWidget *parent = nullptr);
    ~QuizImportDatasourceFormat();

private:
    Ui::QuizImportDatasourceFormat *ui;
};

#endif // QUIZIMPORTDATASOURCEFORMAT_H
