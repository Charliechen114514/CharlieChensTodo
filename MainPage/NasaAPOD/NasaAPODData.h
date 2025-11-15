#ifndef NASAAPODDATA_H
#define NASAAPODDATA_H
#include <QPixmap>

namespace MainPage {
namespace nasa_apod {
	struct NasaAPODData {
		QPixmap image;
		QString image_url;
		QString title;
		QString descriptions;
	};
}
}
#endif // NASAAPODDATA_H
