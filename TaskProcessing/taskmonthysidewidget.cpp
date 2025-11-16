#include "taskmonthysidewidget.h"
#include "button.h"
#include "cclabel.h"
#include <QDate>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QVBoxLayout>
TaskMonthySideWidget::TaskMonthySideWidget(QWidget* parent)
    : QWidget { parent } {
	auto* main = new QVBoxLayout(this);
	main->setSizeConstraint(QLayout::SetMinimumSize);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	CCButton* add_btn = new CCButton("New Monthy Plan +", this);
	add_btn->setFixedHeight(70);
	add_btn->setMaximumWidth(200);
	CCLabel* label = new CCLabel("Choose the Month/Year, or manual refresh!", this);
	label->setMaximumHeight(100);
	CCButton* button = new CCButton("Manial Refresh", this);
	button->setFixedHeight(70);
	button->setMaximumWidth(200);
	// Year row (checkbox + spinbox)
	auto* year_month_row = new QHBoxLayout();
	year_spin = new QSpinBox(this);
	year_spin->setRange(1900, 3000);
	year_spin->setValue(QDate::currentDate().year());
	year_month_row->addWidget(year_spin);
	month_spin = new QSpinBox(this);
	month_spin->setRange(1, 12);
	month_spin->setValue(QDate::currentDate().month());
	year_month_row->addWidget(month_spin);

	// Month row (checkbox + combo or spin)
	main->addWidget(add_btn);
	main->addWidget(label);
	main->addWidget(button);
	main->addLayout(year_month_row);
	CCButton* remove_btn = new CCButton("Delete Current Plan", this);
	remove_btn->setFixedHeight(70);
	remove_btn->setMaximumWidth(200);
	connect(remove_btn, &QPushButton::clicked, this,
	        [this]() {
		        auto ret = QMessageBox::question(
		            this,
		            tr("Delete item"),
		            tr("Are you sure to delete this monthy plan?"));

		        if (ret == QMessageBox::Yes) {
			        emit request_erase_current_plan(composed_value());
		        }
	        });
	main->addWidget(remove_btn);
	main->addStretch();

	connect(month_spin, &QSpinBox::valueChanged, this, [label, this]() {
		emit monthyUserRequest(composed_value());
		label->setText(QString("%1 年 / %2 月的计划")
		                   .arg(year_spin->value())
		                   .arg(month_spin->value()));
	});
	connect(year_spin, &QSpinBox::valueChanged, this, [label, this]() {
		emit monthyUserRequest(composed_value());
		label->setText(QString("%1 年 / %2 月的计划")
		                   .arg(year_spin->value())
		                   .arg(month_spin->value()));
	});
	connect(button, &QPushButton::clicked, this, [label, this]() {
		emit monthyUserRequest(composed_value());
		label->setText(QString("%1 年 / %2 月的计划")
		                   .arg(year_spin->value())
		                   .arg(month_spin->value()));
	});
	connect(add_btn, &QPushButton::clicked, this, &TaskMonthySideWidget::request_new_monthy_plan_add);
	setLayout(main);
}

int TaskMonthySideWidget::composed_value() {
	return year_spin->value() * 100 + month_spin->value();
}
