#ifndef UIEVENTHANDLER_H
#define UIEVENTHANDLER_H

#include <QDate>
#include <QObject>
class TaskRecords;
class TextSelectorDialog;
class DateDialog;
class YearMonthDialog;

class UiEventHandler : public QObject {
	Q_OBJECT
public:
	explicit UiEventHandler(QWidget* parent = nullptr);

	void openFieldsDialog();
	void openDateDialog();
	void openMonthyDialog();

signals:
	void newFieldsTask(const QString field, std::shared_ptr<TaskRecords> t);
	void newDateTask(const QDate date, std::shared_ptr<TaskRecords> t);
	void newMonthyTask(const int month, std::shared_ptr<TaskRecords> t);

private:
	YearMonthDialog* monthy_dialog;
	DateDialog* date_dialog;
	TextSelectorDialog* field_dialog;
};

#endif // UIEVENTHANDLER_H
