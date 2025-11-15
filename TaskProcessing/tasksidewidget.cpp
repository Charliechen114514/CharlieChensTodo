#include "tasksidewidget.h"
#include "CCTextField/cctextfield.h"
#include "button.h"
#include <QCalendarWidget>
#include <QVBoxLayout>

TaskSideWidget::TaskSideWidget(QWidget* parent)
    : QWidget { parent } {
	calendar = new QCalendarWidget(this);
	quotes_fields = new CCTextField(this);
	CCButton* add_btn = new CCButton("Add New+", this);
	connect(add_btn, &QPushButton::clicked,
	        this, &TaskSideWidget::request_new_date_add);
	QVBoxLayout* v = new QVBoxLayout(this);
	v->addWidget(add_btn);
	v->addWidget(calendar);
	v->addWidget(quotes_fields);
	connect(calendar, &QCalendarWidget::clicked,
	        this, &TaskSideWidget::dateUserRequest);
}

void TaskSideWidget::setQuotes(const QStringList& quotes) {
	quotes_fields->setText(quotes.join("\n"));
}
