#include "apoddatadefault.h"
#include <QDir>
#include <QFileDialog>

APODDataDefault::APODDataDefault(QObject* parent)
    : ToDoInitItem { parent } {
}

QString APODDataDefault::itemName() const {
	return "default_cover_folder";
}

bool APODDataDefault::loadIfAlready(const QVariant& v) {
	return true; // do nothings
}

bool APODDataDefault::checkIfIsItemInvalid(const QVariant& v) {
	if (!v.canConvert<QString>())
		return false;
	QString path = v.toString();
	QDir dir(path);
	if (!dir.exists() || path.isEmpty()) {
		qDebug() << "dir is not exsited or unable to handle";
		return true;
	}

	return false;
}

QVariant APODDataDefault::loadIfIsItemInvalid(const QVariant& v) {
	QString folder = QFileDialog::getExistingDirectory(nullptr, "Select a image folder to display",
	                                                   QDir::homePath(),
	                                                   QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (folder.isEmpty())
		return QVariant();

	return folder;
}

QVariant APODDataDefault::loadIfKeyMissing() {
	QString folder = QFileDialog::getExistingDirectory(nullptr, "Select a image folder to display",
	                                                   QDir::homePath(),
	                                                   QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (folder.isEmpty())
		return QVariant();

	return folder;
}

bool APODDataDefault::isCritical() {
	return false;
}
