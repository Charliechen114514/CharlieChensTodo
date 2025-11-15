#include "taskprocessingwidgets.h"
#include "CCToolBox/cctoolbox.h"
#include "Composers/CCTreeWidget/cctreewidget.h"
#include "parser_adapter/markdownparser.h"
#include "task_recordings/taskrecordshelpers.h"
#include "taskfieldsidewidget.h"
#include "tasksidewidget.h"
#include "ui_event/uieventhandler.h"
#include "url_reader.h"
#include <QDate>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QStackedWidget>

TaskProcessingWidgets::TaskProcessingWidgets(QWidget* parent)
    : QWidget { parent } {
	ui_handler = new UiEventHandler(this);
	sideWidget = new TaskSideWidget(this);
	fieldWidget = new TaskFieldSideWidget(this);
	task_lists_displays = new CCTreeWidget(this);
	task_lists_displays->setHeaderLabel("Todays Task");
	task_lists_displays->setColumnCount(1);
	QHBoxLayout* mainView = new QHBoxLayout(this);
	mainView->addWidget(task_lists_displays);
	connect(sideWidget, &TaskSideWidget::dateUserRequest,
	        this, &TaskProcessingWidgets::handle_date_request);
	connect(sideWidget, &TaskSideWidget::request_new_date_add,
	        ui_handler, &UiEventHandler::openDateDialog);

	connect(fieldWidget, &TaskFieldSideWidget::request_new_field_add,
	        ui_handler, &UiEventHandler::openFieldsDialog);
	connect(fieldWidget, &TaskFieldSideWidget::fieldClicked,
	        this, &TaskProcessingWidgets::handle_field_request);

	connect(ui_handler, &UiEventHandler::newFieldsTask,
	        this, &TaskProcessingWidgets::onNewFieldsTask);
	connect(ui_handler, &UiEventHandler::newDateTask,
	        this, &TaskProcessingWidgets::onNewDateTask);
	connect(ui_handler, &UiEventHandler::newMonthyTask,
	        this, &TaskProcessingWidgets::onNewMonthyTask);
}

TaskProcessingWidgets::~TaskProcessingWidgets() {
	rewrite_current_file();
	if (helpers) {
		helpers->deinit();
	}
}

void TaskProcessingWidgets::applyToToolBox(
    CCToolBox* toolbox,
    QStackedWidget* stacked_widget,
    QMap<int, int>& index_mappings) {
	stacked_widget->insertWidget(1, this);
	toolbox->insertItem(
	    1,
	    sideWidget,
	    QIcon(":/toolbox/toolboxs/tasks.png"),
	    "Date Processing");

	toolbox->insertItem(
	    2,
	    fieldWidget,
	    QIcon(":/toolbox/toolboxs/tasks.png"),
	    "Field Tasks");

	// toolbox->insertItem(1, sideWidget, QIcon(":/toolbox/toolboxs/tasks.png"), "Field Sessions");

	index_mappings.insert(1, 1);
	index_mappings.insert(2, 1);
	// index_mappings.insert(3, 1);
}

void TaskProcessingWidgets::parse_markdowns(const QString& markdown_bytes) {
	cutils::PlanParser parser {};
	QStringList quotes {};
	parser.parseMarkdown(markdown_bytes, task_lists_displays, quotes);
	sideWidget->setQuotes(quotes);
	// qDebug().noquote() << parser.fromTreeWidget(task_lists_displays);
}

void TaskProcessingWidgets::handle_date_request(QDate date) {
}

void TaskProcessingWidgets::handle_field_request(QString field) {
	qDebug() << "Activate Fields: " << field;
	rewrite_current_file();
	auto task = helpers->getField(field);
	if (!task) {
		qWarning() << "Failed to find the task";
		QMessageBox::critical(this, "Failed To find the field", "failed to load from remote or local");
		return;
	}
	current_file_path = task->getTaskFilePath();
	try {
		parse_markdowns(cutils::readFromLocalPath(task->getTaskFilePath()));
	} catch (const std::exception& e) {
		QMessageBox::critical(this, "Can not handle!", "Can not handle session: " + task->getTaskFilePath() + " due to" + e.what());
		helpers->removeField(field);
		fieldWidget->removeField(field);
		return;
	}

	fieldWidget->selectField(field);
}

void TaskProcessingWidgets::onNewFieldsTask(const QString field, std::shared_ptr<TaskRecords> t) {

	qDebug() << "Field Add " << field << t->getTaskFilePath();
	rewrite_current_file();
	// Add to the taskHelpers
	helpers->registerField(field, t);
	current_file_path = t->getTaskFilePath();
	// OK, parse from the task handler
	parse_markdowns(cutils::readFromLocalPath(t->getTaskFilePath()));

	fieldWidget->addField(field);
}

void TaskProcessingWidgets::onNewDateTask(const QDate date, std::shared_ptr<TaskRecords> t) {
	qDebug() << "date Add " << date << t->getTaskFilePath();
}

void TaskProcessingWidgets::onNewMonthyTask(const int month, std::shared_ptr<TaskRecords> t) {
	qDebug() << "Month Magic Add " << month << t->getTaskFilePath();
}

void TaskProcessingWidgets::rewrite_current_file() {
	qDebug() << "rewriting the changes..." << current_file_path;
	if (current_file_path.isEmpty()) {
		qDebug() << "Considering the preview mode...";
		return;
	}
	cutils::PlanParser parser {};
	auto str = parser.fromTreeWidget(task_lists_displays);
	if (str.isEmpty()) {
		qDebug() << "Empty strings, dont write!";
		return;
	}
	try {
		cutils::truncate_write(current_file_path, str);
	} catch (const std::exception& e) {
		QMessageBox::critical(this, "Can not rewrite!", QString("Can not rewrite!") + e.what());
		return;
	}
}

void TaskProcessingWidgets::setTaskHelpers(TaskRecordsHelpers* helpers) {
	this->helpers = helpers;
	fieldWidget->setFields(helpers->allFields());
}

void TaskProcessingWidgets::preview_drug(const QUrl& file_path) {
	current_file_path.clear();
	parse_markdowns(cutils::readFromURL(file_path));
}

void TaskProcessingWidgets::collapseAllTasks() {
	task_lists_displays->collapseAll();
}

void TaskProcessingWidgets::expandAllTasks() {
	task_lists_displays->expandAll();
}
