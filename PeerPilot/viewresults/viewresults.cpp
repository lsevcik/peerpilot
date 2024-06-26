#include "viewresults.h"
#include "ui_viewresults.h"
#include "../PeerPilotSurveyReader.h"
#include "../viewquizresults/viewquizresults.h"
#include "../typodetect/typodetect.h"
#include <QFileDialog>
#include <QtSql>
#include <QMessageBox>
#include <utility>

#include <iostream>


viewresults::viewresults(QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::viewresults)
{
	ui->setupUi(this);
	QSqlQueryModel* model = new QSqlQueryModel;
	model->setQuery("SELECT title FROM classes");
	model->setHeaderData(0, Qt::Horizontal, tr("Title"));
	ui->classListView->setModel(model);
	ui->classListView->setModelColumn(0);
	ui->classListView->show();

	connect(ui->importQuizResultsPushButton, &QPushButton::clicked, this, &viewresults::on_importQuizPushButton_clicked);
}

viewresults::~viewresults()
{
	delete ui;
}

int viewresults::on_importQuizPushButton_clicked() {

	QString fileName = QFileDialog::getOpenFileName(this, "Open CSV File", QDir::homePath(), "CSV Files (*.csv)");
	QFile file(fileName);

	// Check if the file exists
	if (!file.exists())
		return QMessageBox::warning(this, "PeerPilot", "File does not exist");

	// Open the file
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return QMessageBox::warning(this, "PeerPilot", "Error opening file");
	// Convert QString to std::string
	std::string filePath = fileName.toStdString();

	//Check file
	// Read the first line of the file
	QByteArray firstLine = file.readLine();
	QString firstLineStr = QString::fromUtf8(firstLine).trimmed();
	std::string firstLineStd = firstLineStr.toStdString();

	// Check if the first line starts with the specified string
	if (firstLineStd.find("name,id,sis_id,root_account,section,section_id,section_sis_id,submitted,attempt") != 0) {
		return QMessageBox::warning(this, "PeerPilot", "Error opening file");
	}

	// Get selected class
	QString className = ui->classListView->selectionModel()->currentIndex().data().toString();

	// Call the getData function with the file path
	ResponseList responses = getData(filePath);

	// Get question titles from data
	std::vector<std::string> titles = getQuestionTitles(filePath);

	// Get students from database
	q.prepare("SELECT students.name FROM students JOIN classes ON students.class_id = classes.id WHERE title=?");
	q.addBindValue(className);
	q.exec();

	while (q.next())
		students.push_back(reformatName(q.value(0).toString().toStdString()));

	// Check for mismatches
	std::vector<std::string> mismatches = responses.getUnmatchedNames(students);

	if (mismatches.empty()) {
		// Open results view
		auto widget = new viewquizresults(this, responses, titles, className);
		widget->show();
		return 0;
	}

	// Get all closest names to the mismatches
	std::vector<std::pair<std::string, std::string>> matchList;
	for (auto& name : mismatches) {
		matchList.push_back(std::make_pair(name, getBestMatchingString(students, name)));
	}

	// Generate message
	QString message = "Unknown names found. The following are automatically generated matchups to known names in the class. Accept?";
	for (auto& namePair : matchList) {
		message.append("<br>" + namePair.first + " -> " + namePair.second);
	}

	if (QMessageBox::question(this, "Mismatches found", message, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
		// Replace all the names
		for (auto& namePair : matchList) {
			responses.replaceName(namePair.first, namePair.second);
		}

		// Open results view
		auto window = new viewquizresults(this, responses, titles, className);
		window->exec();
		return 0;
	}
	//open typodetect
	auto window = new typodetect(this, responses, titles, className, matchList);
	window->exec();

	return 0;
}


