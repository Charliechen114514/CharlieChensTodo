#ifndef NASAAPODATTRWIDGET_H
#define NASAAPODATTRWIDGET_H

#include "NasaAPOD/NasaAPODData.h"
#include <QWidget>
class CCTextField;
class CCLabel;

class NasaAPODAttrWidget : public QWidget {
	Q_OBJECT
public:
	explicit NasaAPODAttrWidget(QWidget* parent = nullptr);

public slots:
	void update_self_info(const MainPage::nasa_apod::NasaAPODData& info);

private:
	CCLabel* title_label;
	CCLabel* img_src_label;
	CCTextField* textField;
};

#endif // NASAAPODATTRWIDGET_H
