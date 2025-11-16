#ifndef DEFAULTIMAGEHANDLER_H
#define DEFAULTIMAGEHANDLER_H
#include "NasaAPOD/NasaAPODData.h"
#include <QDate>
#include <QWidget>

class DefaultImageHandler : public QWidget {
	Q_OBJECT
public:
	explicit DefaultImageHandler(QWidget* parent = nullptr);

	QString getImageFolder() const;
	void setImageFolder(const QString& newImageFolder);
	MainPage::nasa_apod::NasaAPODData getRandomAPOD() const;
	bool isImageFolderAccessible() const;
signals:
	void new_folder_comming(const QString&);
	void requested_date(QDate date);
	void request_show_default(QDate date);
	void postStatus(const QString& p);

private:
	QString imageFolder;
	QStringList titles;
	QStringList descriptions;
};

#endif // DEFAULTIMAGEHANDLER_H
