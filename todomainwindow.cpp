#include "todomainwindow.h"
#include "./ui_todomainwindow.h"
#include "CCToolBox/cctoolbox.h"
#include "TaskProcessing/taskprocessingwidgets.h"
#include "TaskProcessing/ui_event/uieventhandler.h"
#include "init/todopreinithelper.h"
#include "mainpagewidget.h"
#include <QDragEnterEvent>
#include <QFile>
#include <QMimeData>

TodoMainWindow::TodoMainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::TodoMainWindow) {
	ui->setupUi(this);
}

void TodoMainWindow::late_init() {
	setup_toolbox_panel();
	setup_main_page();
	setup_task_page();
	init_connections();
}

void TodoMainWindow::setTaskHandler(TaskRecordsHelpers* helpers) {
	taskWidget->setTaskHelpers(helpers);
}

void TodoMainWindow::setIniter(ToDoPreInitHelper* initer) {
	this->initer = initer;
	late_init_for_initer();
}

TodoMainWindow::~TodoMainWindow() {
	delete ui;
}

void TodoMainWindow::updateIniter(const QString key, const QVariant& v, bool request_update_now) {
	qDebug() << "Update: " << key << " to: " << v;
	initer->update(key, v, request_update_now);
}

void TodoMainWindow::dragEnterEvent(QDragEnterEvent* event) {
	if (event->mimeData()->hasUrls())
		event->acceptProposedAction();
}

void TodoMainWindow::dropEvent(QDropEvent* event) {
	const auto urls = event->mimeData()->urls();
	taskWidget->preview_drug(urls.last());
	event->acceptProposedAction();
}

void TodoMainWindow::setup_toolbox_panel() {
	QHBoxLayout* l = dynamic_cast<QHBoxLayout*>(ui->centralwidget->layout());
	l->setStretchFactor(ui->stackedWidget, 2);
	QWidget* toolbox_container = new QWidget(ui->centralwidget);
	QHBoxLayout* h = new QHBoxLayout(toolbox_container);
	box = new CCToolBox(ui->centralwidget);
	h->addWidget(box);
	l->addWidget(toolbox_container);
	l->setStretchFactor(toolbox_container, 1);
}

void TodoMainWindow::setup_main_page() {
	mainPageWidget = new MainPageWidget(ui->stackedWidget);
	mainPageWidget->applyToToolBox(box, ui->stackedWidget, index_mappings);
}

void TodoMainWindow::setup_task_page() {
	taskWidget = new TaskProcessingWidgets(ui->stackedWidget);
	taskWidget->applyToToolBox(box, ui->stackedWidget, index_mappings);
}

void TodoMainWindow::init_connections() {
	connect(box, &QToolBox::currentChanged, ui->stackedWidget, [this](int index) {
		int mappings = index_mappings.value(index);
		ui->stackedWidget->setCurrentIndex(mappings);
	});

	connect(taskWidget, &TaskProcessingWidgets::postStatus,
	        this, [this](const QString& st) {
		        ui->statusbar->showMessage(st);
	        });
	connect(mainPageWidget, &MainPageWidget::postStatus,
	        this, [this](const QString& st) {
		        ui->statusbar->showMessage(st);
	        });

	UiEventHandler* event_handler = taskWidget->ui_event_handler();
	auto con_actions = [=](QAction* act, void (UiEventHandler::*action)(void)) {
		connect(act, &QAction::triggered, event_handler, action);
	};

	con_actions(ui->action_addNewDate, &UiEventHandler::openDateDialog);
	con_actions(ui->action_addNewField, &UiEventHandler::openFieldsDialog);
	con_actions(ui->action_addNewMonthy, &UiEventHandler::openMonthyDialog);
}

void TodoMainWindow::late_init_for_initer() {
	auto res = initer->query("default_cover_folder");
	if (!res.isValid()) {
		qDebug() << "Not a valid covers...";
		return;
	} else {
		mainPageWidget->setDefault_image_folder(res.toString());
	}
}
