#include "textselectordialog.h"
#include "CCTextField/cctextfield.h"
#include "button.h"
#include "cclabel.h"
#include "commons.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

TextSelectorDialog::TextSelectorDialog(QWidget* parent) {
	setWindowTitle(tr("Enter text and select Markdown"));
	auto* main = new QVBoxLayout(this);

	// Text input
	auto* textLabel = new QLabel(tr("Text:"), this);
	textField = new CCTextField(this);
	main->addWidget(textLabel);
	main->addWidget(textField);

	// File select
	auto* fileRow = new QHBoxLayout();
	fileLabel_ = new CCLabel("(no file selected)", this);
	fileLabel_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	auto* fileBtn = new QPushButton(tr("Choose Markdown..."), this);
	fileRow->addWidget(fileBtn);
	fileRow->addWidget(fileLabel_);
	main->addLayout(fileRow);

	// Buttons: Confirm, Clear, Cancel
	auto* box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
	clearButton = new CCButton("Clear", this);
	box->addButton(clearButton, QDialogButtonBox::ResetRole);
	main->addWidget(box);

	// initial state
	box->button(QDialogButtonBox::Ok)->setEnabled(false);

	connect(fileBtn, &QPushButton::clicked, this, &TextSelectorDialog::onChooseFile);
	connect(textField, &CCTextField::textChanged, this, &TextSelectorDialog::updateOkState);
	connect(clearButton, &QPushButton::clicked, this, &TextSelectorDialog::onClear);
	connect(box, &QDialogButtonBox::accepted, this, &TextSelectorDialog::onAccept);
	connect(box, &QDialogButtonBox::rejected, this, &TextSelectorDialog::onCancel);
}

void TextSelectorDialog::onChooseFile() {
	QString f = QFileDialog::getOpenFileName(this, tr("Select Markdown file"), QString(), tr("Markdown Files (*.md *.markdown);;All Files (*)"));
	if (!f.isEmpty()) {
		filePath_ = f;
		fileLabel_->setText(f);
	}
	updateOkState();
}

void TextSelectorDialog::onClear() {
	textField->setText("");
	filePath_.clear();
	fileLabel_->setText(tr("(no file selected)"));
	updateOkState();
}

void TextSelectorDialog::onAccept() {
	if (!cutils::dialog_helpers::isValidMarkdownPath(filePath_)) {
		QMessageBox::warning(this, tr("Invalid file"), tr("Please choose an existing Markdown file (*.md, *.markdown)."));
		return;
	}
	// emit signal and close
	emit confirmed(textField->text(), filePath_);
	onClear();
	accept();
}

void TextSelectorDialog::onCancel() {
	reject();
}

void TextSelectorDialog::updateOkState() {
	bool ok = !textField->text().isEmpty() && cutils::dialog_helpers::isValidMarkdownPath(filePath_);
	auto* box = findChild<QDialogButtonBox*>();
	if (box)
		box->button(QDialogButtonBox::Ok)->setEnabled(ok);
}
