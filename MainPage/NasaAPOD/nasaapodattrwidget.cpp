#include "nasaapodattrwidget.h"
#include "CCTextField/cctextfield.h"
#include "cclabel.h"
#include <QVBoxLayout>

NasaAPODAttrWidget::NasaAPODAttrWidget(QWidget* parent)
    : QWidget { parent } {
	QVBoxLayout* v = new QVBoxLayout(this);
	v->setContentsMargins(8, 6, 8, 6);
	v->addSpacing(1);
	title_label = new CCLabel(this);
	title_label->setWordWrap(true);
	v->addWidget(title_label);
	v->setStretchFactor(title_label, 1);
	textField = new CCTextField(this);
	textField->setMultiline(true);
	v->addWidget(textField);
	v->setStretchFactor(textField, 5);

	img_src_label = new CCLabel(this);
	img_src_label->setWordWrap(true);
	v->addWidget(img_src_label);
	v->setStretchFactor(img_src_label, 1);
}

void NasaAPODAttrWidget::update_self_info(
    const MainPage::nasa_apod::NasaAPODData& info) {
	title_label->setText(info.title);
	img_src_label->setText(info.image_url);
	textField->setText(info.descriptions);
}
