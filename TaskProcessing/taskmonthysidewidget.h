#ifndef TASKMONTHYSIDEWIDGET_H
#define TASKMONTHYSIDEWIDGET_H
#include <QSpinBox>
#include <QWidget>

class TaskMonthySideWidget : public QWidget {
	Q_OBJECT
public:
	explicit TaskMonthySideWidget(QWidget* parent = nullptr);

signals:
	void request_new_monthy_plan_add();
	void request_erase_current_plan(int request_id);
	void monthyUserRequest(int request_id);

private:
	int composed_value();

private:
	QSpinBox* year_spin;
	QSpinBox* month_spin;
};

#endif // TASKMONTHYSIDEWIDGET_H
