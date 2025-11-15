#include "yearmonthdialog.h"
#include "button.h"
#include "cclabel.h"
#include "commons.h"
#include <QCheckBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

YearMonthDialog::YearMonthDialog(QWidget* parent)
    : QDialog(parent) {
	setWindowTitle(tr("Select Year/Month and Markdown"));
	auto* main = new QVBoxLayout(this);

	// Year row (checkbox + spinbox)
	auto* yearRow = new QHBoxLayout();
	yearCheck = new QCheckBox(tr("Use Year"), this);
	yearSpin_ = new QSpinBox(this);
	yearSpin_->setRange(1900, 3000);
	yearSpin_->setValue(QDate::currentDate().year());
	yearSpin_->setEnabled(false);
	yearRow->addWidget(yearCheck);
	yearRow->addWidget(yearSpin_);
	main->addLayout(yearRow);

	// Month row (checkbox + combo or spin)
	auto* monthRow = new QHBoxLayout();
	month_check = new QCheckBox(tr("Use Month"), this);
	month_spin = new QSpinBox(this);
	month_spin->setRange(1, 12);
	month_spin->setValue(QDate::currentDate().month());
	month_spin->setEnabled(false);
	monthRow->addWidget(month_check);
	monthRow->addWidget(month_spin);
	main->addLayout(monthRow);

	// File select
	auto* fileRow = new QHBoxLayout();
	fileLabel_ = new CCLabel(tr("(no file selected)"), this);
	fileLabel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	auto* fileBtn = new QPushButton(tr("Choose Markdown..."), this);
	fileRow->addWidget(fileBtn);
	fileRow->addWidget(fileLabel_);
	main->addLayout(fileRow);

	// Buttons
	auto* box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
	clearButton = new CCButton(tr("Clear"), this);
	box->addButton(clearButton, QDialogButtonBox::ResetRole);
	main->addWidget(box);

	box->button(QDialogButtonBox::Ok)->setEnabled(false);

	connect(yearCheck, &QCheckBox::toggled, yearSpin_, &QSpinBox::setEnabled);
	connect(month_check, &QCheckBox::toggled, month_spin, &QSpinBox::setEnabled);

	connect(fileBtn, &QPushButton::clicked, this, &YearMonthDialog::onChooseFile);
	connect(clearButton, &QPushButton::clicked, this, &YearMonthDialog::onClear);
	connect(box, &QDialogButtonBox::accepted, this, &YearMonthDialog::onAccept);
	connect(box, &QDialogButtonBox::rejected, this, &YearMonthDialog::onCancel);

	// if user toggles year/month or file, update ok state
	connect(yearCheck, &QCheckBox::toggled, this, &YearMonthDialog::updateOkState);
	connect(month_check, &QCheckBox::toggled, this, &YearMonthDialog::updateOkState);
}

void YearMonthDialog::onChooseFile() {
	QString f = QFileDialog::getOpenFileName(this, tr("Select Markdown file"), QString(), tr("Markdown Files (*.md *.markdown);;All Files (*)"));
	if (!f.isEmpty()) {
		filePath_ = f;
		fileLabel_->setText(f);
	}
	updateOkState();
}

void YearMonthDialog::onClear() {
	yearCheck->setChecked(false);
	month_check->setChecked(false);
	yearSpin_->setValue(QDate::currentDate().year());
	month_spin->setValue(QDate::currentDate().month());
	filePath_.clear();
	fileLabel_->setText(tr("(no file selected)"));
	updateOkState();
}

void YearMonthDialog::onAccept() {
	if (!cutils::dialog_helpers::isValidMarkdownPath(filePath_)) {
		QMessageBox::warning(this, tr("Invalid file"), tr("Please choose an existing Markdown file (*.md, *.markdown)."));
		return;
	}
	// Require at least one of year or month selected
	if (!yearCheck->isChecked() && !month_check->isChecked()) {
		QMessageBox::warning(this, tr("Select Year/Month"), tr("Please enable at least Year or Month selection by checking the box(es)."));
		return;
	}
	emit confirmed(yearSpin_->value() * 100 + month_spin->value(), filePath_);
	accept();
	onClear();
}

void YearMonthDialog::onCancel() {
	reject();
}

void YearMonthDialog::updateOkState() {
	bool fileOk = cutils::dialog_helpers::isValidMarkdownPath(filePath_);
	bool selectionOk = (yearCheck->isChecked() || month_check->isChecked());
	auto* box = findChild<QDialogButtonBox*>();
	if (box)
		box->button(QDialogButtonBox::Ok)->setEnabled(fileOk && selectionOk);
}
