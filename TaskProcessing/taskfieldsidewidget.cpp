#include "taskfieldsidewidget.h"
#include "CCButton/button.h"
#include <QVBoxLayout>
TaskFieldSideWidget::TaskFieldSideWidget(QWidget* parent)
    : QWidget { parent } {
	QVBoxLayout* sum_layout = new QVBoxLayout(this);
	CCButton* add_btn = new CCButton("Add New +", this);
	sum_layout->addWidget(add_btn);
	connect(add_btn, &QPushButton::clicked,
	        this, &TaskFieldSideWidget::request_new_field_add);
	v = new QVBoxLayout;
	setLayout(v);
	v->setContentsMargins(4, 4, 4, 4);
	v->setSpacing(6);
	sum_layout->addLayout(v);
}

void TaskFieldSideWidget::setFields(const QStringList& l) {
	clearButtons();

	for (const QString& f : l) {
		CCButton* btn = new CCButton(f, this);
		btn->setCheckable(true);
		btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
		btn->setCursor(Qt::PointingHandCursor);

		connect(btn, &QPushButton::clicked, this, [this, f]() {
			selectField(f);
			emit fieldClicked(f);
		});

		v->addWidget(btn);
		buttons_.insert(f, btn);
	}

	v->addStretch(1);
}

void TaskFieldSideWidget::addField(const QString& field) {
	if (buttons_.contains(field))
		return;

	CCButton* btn = new CCButton(field, this);
	btn->setCheckable(true);
	btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	btn->setCursor(Qt::PointingHandCursor);

	connect(btn, &QPushButton::clicked, this, [this, field]() {
		selectField(field);
		emit fieldClicked(field);
	});

	v->insertWidget(v->count() - 1, btn); // 保证 stretch 在最后
	buttons_.insert(field, btn);
}

void TaskFieldSideWidget::removeField(const QString& field) {
	if (!buttons_.contains(field))
		return;

	if (currentField_ == field)
		currentField_.clear();

	CCButton* btn = buttons_.take(field);
	btn->disconnect();
	btn->hide();
	btn->deleteLater();
}

void TaskFieldSideWidget::selectField(const QString& field) {
	if (field == currentField_)
		return;

	if (!currentField_.isEmpty()) {
		if (auto prev = buttons_.value(currentField_, nullptr))
			prev->setChecked(false);
	}

	if (auto btn = buttons_.value(field, nullptr)) {
		btn->setChecked(true);
		btn->ensurePolished();
		btn->update();
	} else {
		qDebug() << "TaskFieldSideWidget::selectField: unknown field" << field;
	}

	currentField_ = field;
}

void TaskFieldSideWidget::clearButtons() {
	while (QLayoutItem* it = v->takeAt(0)) {
		QWidget* w = it->widget();
		if (w) {
			w->disconnect();
			w->hide();
			w->deleteLater();
		}
		delete it;
	}
	buttons_.clear();
	currentField_.clear();
}
