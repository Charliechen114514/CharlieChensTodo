#ifndef MAINPAGEWIDGET_H
#define MAINPAGEWIDGET_H
#include "../interfaces/IPagedWidget.h"
#include "NasaAPOD/NasaAPODData.h"
#include <QFutureWatcher>
#include <QWidget>

namespace MainPage::nasa_apod {
class NasaAPODFetcher;
}
namespace CCWidgetLibrary::animation {
class AppearAnimation;
}
class CCToolBox;
class CCImageWidget;
class FloatingLabelHelper;
class QPushButton;
class QStackedWidget;

class MainPageWidget : public QWidget, IPagedWidget {
	Q_OBJECT
public:
	explicit MainPageWidget(QWidget* parent = nullptr);
	~MainPageWidget();
	void flushMainPage();
	void applyToToolBox(CCToolBox* toolbox,
	                    QStackedWidget* stacked_widget,
	                    QMap<int, int>& index_mappings) override;

signals:
	void update_nasa_data(const MainPage::nasa_apod::NasaAPODData& data);

protected:
	void resizeEvent(QResizeEvent* event) override;
	bool eventFilter(QObject* watched, QEvent* event) override;

private:
	void handle_failed();
	void handle_data(const MainPage::nasa_apod::NasaAPODData& data);
	void updateFloatingPos();

private: // init field
	void init_core();
	void init_ui();
	void init_connections();

private:
	using DataFuture_t = std::optional<MainPage::nasa_apod::NasaAPODData>;
	QFutureWatcher<DataFuture_t>* watcher;
	CCImageWidget* cover_widget;
	QPushButton* floatings_widget { nullptr };
	FloatingLabelHelper* title_label_helper;
	FloatingLabelHelper* main_label_helper;
	MainPage::nasa_apod::NasaAPODFetcher* fetcher;
	CCWidgetLibrary::animation::AppearAnimation* floats_main_animation;
	CCWidgetLibrary::animation::AppearAnimation* label_appear_animation;
};

#endif // MAINPAGEWIDGET_H
