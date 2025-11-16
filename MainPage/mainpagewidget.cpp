#include "mainpagewidget.h"
#include "CCImageWidget/ccimagewidget.h"
#include "CCToolBox/cctoolbox.h"
#include "NasaAPOD/nasaapodattrwidget.h"
#include "NasaAPOD/nasaapodfetcher.h"
#include "button.h"
#include "defaultimagehandler.h"
#include "floatinglabelhelper.h"
#include "helpers/animations/appearanimation.h"
#include <QFutureWatcher>
#include <QHBoxLayout>
#include <QStackedWidget>

MainPageWidget::MainPageWidget(QWidget* parent)
    : QWidget { parent } {
	init_core();
	init_ui();
	init_connections();
}

void MainPageWidget::init_core() {
	watcher = new QFutureWatcher<DataFuture_t>(this);
	connect(watcher, &QFutureWatcher<DataFuture_t>::finished, this, [this]() {
		DataFuture_t result = watcher->future().result();
		if (!result) {
			handle_failed();
		} else {
			handle_data(*result);
		}
	});
	fetcher = new MainPage::nasa_apod::NasaAPODFetcher;
}

void MainPageWidget::init_ui() {
	default_handler = new DefaultImageHandler(this);
	QHBoxLayout* layout
	    = new QHBoxLayout(this);
	cover_widget = new CCImageWidget(this);
	cover_widget->setScale_mode(CCImageWidget::Cover);
	layout->addWidget(cover_widget);
	floatings_widget = new CCButton(this);
	floatings_widget->setMouseTracking(true);
	floatings_widget->setFixedSize(50, 50);
	floatings_widget->setText("🔍");
	title_label_helper = new FloatingLabelHelper(this);
	main_label_helper = new FloatingLabelHelper(this);
	main_label_helper->setFontSize(10);
	label_appear_animation
	    = new CCWidgetLibrary::animation::AppearAnimation(title_label_helper);
	label_appear_animation->apply_config({ QEasingCurve::InOutCubic, 600 });
	floats_main_animation
	    = new CCWidgetLibrary::animation::AppearAnimation(main_label_helper);
	floats_main_animation->apply_config({ QEasingCurve::InOutCubic, 600 });
	flushMainPage();
	auto datas = default_handler->getRandomAPOD();
	handle_data(datas);
}

void MainPageWidget::init_connections() {
	floatings_widget->installEventFilter(this);
	connect(default_handler, &DefaultImageHandler::postStatus,
	        this, &MainPageWidget::postStatus);
	connect(default_handler, &DefaultImageHandler::requested_date,
	        this, &MainPageWidget::flushMainPage);
	connect(default_handler, &DefaultImageHandler::request_show_default,
	        this, &MainPageWidget::handle_show_default);
	connect(floatings_widget, &QPushButton::clicked,
	        this, [this]() {
		        if (main_label_helper->isVisible()) {
			        floats_main_animation->disappear();
		        } else {
			        floats_main_animation->appear();
		        }
	        });
	connect(floats_main_animation, &CCWidgetLibrary::animation::AppearAnimation::finished,
	        this, [this]() {
		        updateFloatingPos();
	        });
	connect(label_appear_animation, &CCWidgetLibrary::animation::AppearAnimation::finished,
	        this, [this]() { updateFloatingPos(); });
}

bool MainPageWidget::eventFilter(QObject* watched, QEvent* event) {
	if (watched == floatings_widget) {
		if (event->type() == QEvent::Enter) {
			label_appear_animation->appear();
			return true;
		} else if (event->type() == QEvent::Leave) {
			label_appear_animation->disappear();
			return true;
		}
	}
	return QWidget::eventFilter(watched, event);
}

void MainPageWidget::flushMainPage(QDate request_date) {
	title_label_helper->displayBusyText();
	main_label_helper->displayBusyText();
	fetcher->setDate(request_date);
	emit postStatus("Flushing the date: " + request_date.toString() + "...");
	auto future = fetcher->factorized();
	watcher->setFuture(future);
}

void MainPageWidget::applyToToolBox(CCToolBox* toolbox,
                                    QStackedWidget* stacked_widget,
                                    QMap<int, int>& index_mappings) {
	QWidget* container = new QWidget(this);
	QVBoxLayout* v = new QVBoxLayout(container);
	v->addWidget(default_handler);
	NasaAPODAttrWidget* widget
	    = new NasaAPODAttrWidget(this);
	v->addWidget(widget);
	connect(this, &MainPageWidget::update_nasa_data,
	        widget, &NasaAPODAttrWidget::update_self_info);

	stacked_widget->insertWidget(0, this);
	toolbox->insertItem(0, container, QIcon(":/toolbox/toolboxs/space.png"), "MainPage");
	index_mappings.insert(0, 0);
}

QString MainPageWidget::getDefault_image_folder() const {
	return default_handler->getImageFolder();
}

void MainPageWidget::setDefault_image_folder(const QString& newDefault_image_folder) {
	if (newDefault_image_folder == default_handler->getImageFolder()) {
		return;
	}

	default_handler->setImageFolder(newDefault_image_folder);
	emit request_update_imageFolderPath("default_cover_folder", newDefault_image_folder, true);
}

void MainPageWidget::resizeEvent(QResizeEvent* event) {
	QWidget::resizeEvent(event);
	main_label_helper->setMaximumWidth(width() * 3 / 4);
	updateFloatingPos();
}

void MainPageWidget::handle_failed() {
	qDebug() << "Error Occurs when attempting fetching, get from defaults";
	auto datas = default_handler->getRandomAPOD();
	handle_data(datas);
}

void MainPageWidget::
    handle_data(const MainPage::nasa_apod::NasaAPODData& data) {
	emit update_nasa_data(data);
	cover_widget->setPixmap(data.image);
	title_label_helper->setTitleText(data.title);
	main_label_helper->setTitleText(data.descriptions);
	updateFloatingPos();
	main_label_helper->auto_adjust_text_color(cover_widget);
	title_label_helper->auto_adjust_text_color(cover_widget);
}

void MainPageWidget::handle_show_default(const QDate& d) {
	auto datas = default_handler->getRandomAPOD();
	handle_data(datas);
}

void MainPageWidget::updateFloatingPos() {
	if (!floatings_widget)
		return;

	int btnX = width() - floatings_widget->width() - 50;
	int btnY = height() - floatings_widget->height() - 20;
	floatings_widget->move(btnX, btnY);
	title_label_helper->adjustSize();
	main_label_helper->adjustSize();

	int labelX_local = btnX + floatings_widget->width() - title_label_helper->width();
	int labelY_local = btnY - title_label_helper->height() - 10;

	int main_x_local = btnX + floatings_widget->width() - main_label_helper->width();
	int main_y_local = btnY - main_label_helper->height() - 10;

	label_appear_animation->setStartPos(QPoint(labelX_local, btnY));
	label_appear_animation->setEndPos(QPoint(labelX_local, labelY_local));
	title_label_helper->move(labelX_local, labelY_local);

	floats_main_animation->setStartPos(QPoint(main_x_local, 2 * btnY));
	floats_main_animation->setEndPos(QPoint(main_x_local, main_y_local));
	main_label_helper->move(main_x_local, main_y_local);
}

MainPageWidget::~MainPageWidget() {
	watcher->waitForFinished();
}
