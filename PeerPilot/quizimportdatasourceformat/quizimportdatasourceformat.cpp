#include "quizimportdatasourceformat.h"
#include "ui_quizimportdatasourceformat.h"

QuizImportDatasourceFormat::QuizImportDatasourceFormat(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QuizImportDatasourceFormat)
{
    ui->setupUi(this);
}

QuizImportDatasourceFormat::~QuizImportDatasourceFormat()
{
    delete ui;
}
