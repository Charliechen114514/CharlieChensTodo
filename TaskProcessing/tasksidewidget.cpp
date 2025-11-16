#include "tasksidewidget.h"
#include "CCTextField/cctextfield.h"
#include "button.h"
#include "cclabel.h"
#include <QCalendarWidget>
#include <QMessageBox>
#include <QVBoxLayout>

TaskSideWidget::TaskSideWidget(QWidget* parent)
    : QWidget { parent } {
	calendar = new QCalendarWidget(this);
	quotes_fields = new CCTextField(this);
	CCButton* add_btn = new CCButton("Add New+", this);
	connect(add_btn, &QPushButton::clicked,
	        this, &TaskSideWidget::request_new_date_add);
	CCButton* remove_btn = new CCButton("Delete Current Plan", this);
	remove_btn->setFixedHeight(70);
	remove_btn->setMaximumWidth(200);
	connect(remove_btn, &QPushButton::clicked, this,
	        [this]() {
		        QDate deleted_date = calendar->selectedDate();
		        if (!deleted_date.isValid()) {
			        QMessageBox::critical(this, "Not a valid date",
			                              "You haven't select a valid date");
			        return;
		        }

		        auto ret = QMessageBox::question(
		            this,
		            tr("Delete item"),
		            "Are you sure to delete date: " + calendar->selectedDate().toString() + "'s plan?");

		        if (ret == QMessageBox::Yes) {
			        emit request_erase_current_plan(deleted_date);
		        }
	        });
	CCLabel* dateLabel = new CCLabel("Cur: ", this);
	QVBoxLayout* v = new QVBoxLayout(this);
	v->addWidget(add_btn);
	v->addWidget(dateLabel);
	v->addWidget(calendar);
	v->addWidget(quotes_fields);
	v->addWidget(remove_btn);
	connect(calendar, &QCalendarWidget::clicked,
	        this, &TaskSideWidget::dateUserRequest);
	connect(this, &TaskSideWidget::dateUserRequest,
	        this, [dateLabel](QDate d) {
		        dateLabel->setText("Cur: " + d.toString("yyyy / MMM / d"));
	        });
}

void TaskSideWidget::setQuotes(const QStringList& quotes) {
	quotes_fields->setText(quotes.join("\n"));
}
