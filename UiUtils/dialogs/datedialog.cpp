#include "datedialog.h"
#include "button.h"
#include "cclabel.h"
#include "commons.h"
#include <QCalendarWidget>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>

DateDialog::DateDialog(QWidget* parent)
    : QDialog(parent) {
	setWindowTitle(tr("Pick a date and select Markdown"));
	auto* main = new QVBoxLayout(this);

	// Calendar and DateEdit
	auto* dateRow = new QHBoxLayout();
	calendar_ = new QCalendarWidget(this);
	dateEdit_ = new QDateEdit(QDate::currentDate(), this);
	dateEdit_->setCalendarPopup(true);
	dateRow->addWidget(calendar_, 1);
	auto* rightCol = new QVBoxLayout();
	rightCol->addWidget(new CCLabel(tr("Date (editable):"), this));
	rightCol->addWidget(dateEdit_);
	dateRow->addLayout(rightCol);
	main->addLayout(dateRow);

	// File select
	auto* fileRow = new QHBoxLayout();
	fileLabel_ = new CCLabel(tr("(no file selected)"), this);
	fileLabel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	auto* fileBtn = new CCButton(tr("Choose Markdown..."), this);
	fileRow->addWidget(fileBtn);
	fileRow->addWidget(fileLabel_);
	main->addLayout(fileRow);

	// Buttons
	auto* box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
	clearButton_ = new QPushButton(tr("Clear"), this);
	box->addButton(clearButton_, QDialogButtonBox::ResetRole);
	main->addWidget(box);

	// initial
	box->button(QDialogButtonBox::Ok)->setEnabled(false);

	// sync calendar <-> dateedit
	connect(calendar_, &QCalendarWidget::selectionChanged, this, [this]() {
		QSignalBlocker b(dateEdit_);
		dateEdit_->setDate(calendar_->selectedDate());
	});
	connect(dateEdit_, &QDateEdit::dateChanged, this, [this](const QDate& d) {
		QSignalBlocker b(calendar_);
		calendar_->setSelectedDate(d);
	});

	connect(fileBtn, &QPushButton::clicked, this, &DateDialog::onChooseFile);
	connect(clearButton_, &QPushButton::clicked, this, &DateDialog::onClear);
	connect(box, &QDialogButtonBox::accepted, this, &DateDialog::onAccept);
	connect(box, &QDialogButtonBox::rejected, this, &DateDialog::onCancel);
}

void DateDialog::onChooseFile() {
	QString f = QFileDialog::getOpenFileName(this, tr("Select Markdown file"), QString(), tr("Markdown Files (*.md *.markdown);;All Files (*)"));
	if (!f.isEmpty()) {
		file_path = f;
		fileLabel_->setText(f);
	}
	updateOkState();
}

void DateDialog::onClear() {
	// reset date to today
	dateEdit_->setDate(QDate::currentDate());
	calendar_->setSelectedDate(QDate::currentDate());
	file_path.clear();
	fileLabel_->setText(tr("(no file selected)"));
	updateOkState();
}

void DateDialog::onAccept() {
	if (!cutils::dialog_helpers::isValidMarkdownPath(file_path)) {
		QMessageBox::warning(this, tr("Invalid file"), tr("Please choose an existing Markdown file (*.md, *.markdown)."));
		return;
	}
	// emit signal and close
	emit confirmed(dateEdit_->date(), file_path);
	onClear();
	accept();
}

void DateDialog::onCancel() {
	reject();
}

void DateDialog::updateOkState() {
	bool ok = cutils::dialog_helpers::isValidMarkdownPath(file_path);
	auto* box = findChild<QDialogButtonBox*>();
	if (box)
		box->button(QDialogButtonBox::Ok)->setEnabled(ok);
}
