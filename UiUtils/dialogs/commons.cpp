#include "commons.h"
#include <QFileInfo>
#include <QString>
bool cutils::dialog_helpers::isValidMarkdownPath(const QString& p) {
	if (p.isEmpty())
		return false;
	QFileInfo fi(p);
	if (!fi.exists() || !fi.isFile())
		return false;
	QString suffix = fi.suffix().toLower();
	return (suffix == "md" || suffix == "markdown");
}
