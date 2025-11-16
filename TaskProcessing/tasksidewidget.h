#ifndef TASKSIDEWIDGET_H
#define TASKSIDEWIDGET_H

#include <QWidget>
class CCTextField;
class QCalendarWidget;

class TaskSideWidget : public QWidget {
	Q_OBJECT
public:
	explicit TaskSideWidget(QWidget* parent = nullptr);
	void setQuotes(const QStringList& quotes);
signals:
	void request_new_date_add();
	void dateUserRequest(QDate date);
	void request_erase_current_plan(QDate date);

private:
	CCTextField* quotes_fields;
	QCalendarWidget* calendar;
};

#endif // TASKSIDEWIDGET_H
