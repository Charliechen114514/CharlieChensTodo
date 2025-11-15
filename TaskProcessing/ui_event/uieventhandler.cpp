#include "uieventhandler.h"
#include "../task_recordings/taskrecords.h"
#include "dialogs/datedialog.h"
#include "dialogs/textselectordialog.h"
#include "dialogs/yearmonthdialog.h"
#include <QWidget>
UiEventHandler::UiEventHandler(QWidget* parent)
    : QObject { parent } {
	date_dialog = new DateDialog(parent);
	monthy_dialog = new YearMonthDialog(parent);
	field_dialog = new TextSelectorDialog(parent);

	connect(date_dialog, &DateDialog::confirmed,
	        this, [this](const QDate& d, const QString& markdown_path) {
		        emit newDateTask(d, std::make_shared<TaskRecords>(markdown_path));
	        });
	connect(monthy_dialog, &YearMonthDialog::confirmed,
	        this, [this](const int& d, const QString& markdown_path) {
		        emit newMonthyTask(d, std::make_shared<TaskRecords>(markdown_path));
	        });
	connect(field_dialog, &TextSelectorDialog::confirmed,
	        this, [this](const QString& d, const QString& markdown_path) {
		        emit newFieldsTask(d, std::make_shared<TaskRecords>(markdown_path));
	        });
}

void UiEventHandler::openFieldsDialog() {
	field_dialog->open();
}

void UiEventHandler::openDateDialog() {
	date_dialog->open();
}

void UiEventHandler::openMonthyDialog() {
	monthy_dialog->open();
}
