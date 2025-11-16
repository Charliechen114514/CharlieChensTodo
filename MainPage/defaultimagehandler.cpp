#include "defaultimagehandler.h"
#include "CCCalendarWidget/cccalendarwidget.h"
#include "button.h"
#include "cclabel.h"
#include <QCalendarWidget>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QRandomGenerator>
#include <QVBoxLayout>

DefaultImageHandler::DefaultImageHandler(QWidget* parent)
    : QWidget { parent } {

	QVBoxLayout* v = new QVBoxLayout(this);

	CCButton* setButtons = new CCButton("Set Default Caches Image Library", this);
	setButtons->setFixedHeight(60);
	connect(setButtons, &CCButton::clicked, this, [this]() {
		QString folder = QFileDialog::getExistingDirectory(
		    this,
		    "Select Image Library Folder",
		    QDir::homePath());

		if (folder.isEmpty()) {
			return;
		}

		setImageFolder(folder);
	});

	CCLabel* label = new CCLabel("Current: ", this);
	CCCalendarWidget* cw = new CCCalendarWidget(this);

	connect(cw, &QCalendarWidget::clicked,
	        this, &DefaultImageHandler::requested_date);

	CCButton* default_btn = new CCButton("See Default", this);
	default_btn->setFixedHeight(60);
	connect(default_btn, &QPushButton::clicked,
	        this, [this, cw]() {
		        emit request_show_default(cw->selectedDate());
	        });

	v->addWidget(cw);
	v->addWidget(setButtons);
	v->addWidget(default_btn);
	v->addWidget(label);

	titles = {
		"Starlit Horizons",
		"Cosmic Dawn",
		"Galactic Mystery",
		"Nebula Dreams",
		"Journey Through Space"
	};

	descriptions = {
		"A breathtaking look into the vast unknown of space.",
		"A rare celestial moment captured from the depths of the universe.",
		"An awe-inspiring phenomenon illuminating the cosmic dark.",
		"Colors and patterns revealing the life cycle of distant stars.",
		"A moment frozen in time, light-years away from Earth."
	};
}

QString DefaultImageHandler::getImageFolder() const {
	return imageFolder;
}

void DefaultImageHandler::setImageFolder(const QString& newImageFolder) {
	if (newImageFolder == imageFolder) {
		return;
	}
	imageFolder = newImageFolder;
	emit postStatus("Set to the" + newImageFolder + "as default Images Folder");
	emit new_folder_comming(newImageFolder);
}

MainPage::nasa_apod::NasaAPODData DefaultImageHandler::getRandomAPOD() const {

	MainPage::nasa_apod::NasaAPODData data;

	QDir dir(imageFolder);
	if (!isImageFolderAccessible()) {
		qWarning() << "Image folder does not exist:" << imageFolder;
		return data;
	}

	QStringList filters = { "*.png", "*.jpg", "*.jpeg", "*.bmp", "*.webp" };
	dir.setNameFilters(filters);

	QFileInfoList list = dir.entryInfoList(QDir::Files);
	if (list.isEmpty()) {
		qWarning() << "No image files found in:" << imageFolder;
		return data;
	}

	int index = QRandomGenerator::global()->bounded(list.size());
	const QFileInfo& info = list.at(index);

	QPixmap pix(info.absoluteFilePath());
	if (!pix.isNull()) {
		data.image = pix;
		data.image_url = info.absoluteFilePath();
	}

	if (!titles.isEmpty()) {
		int ti = QRandomGenerator::global()->bounded(titles.size());
		data.title = titles.at(ti);
	}
	if (!descriptions.isEmpty()) {
		int di = QRandomGenerator::global()->bounded(descriptions.size());
		data.descriptions = descriptions.at(di);
	}

	return data;
}

bool DefaultImageHandler::isImageFolderAccessible() const {
	QDir dir(imageFolder);
	if (!dir.exists()) {
		return false;
	}
	return true;
}
