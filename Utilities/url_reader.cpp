#include "url_reader.h"
#include <QFile>
#include <QTextStream>
#include <QUrl>
QString cutils::readFromURL(const QUrl& url) {
	if (url.isLocalFile()) {
		return readFromLocalPath(url.toLocalFile());
	}
	throw std::runtime_error("Current can not handle non local file");
}

QString cutils::readFromLocalPath(const QString& path) {
	QFile f(path);
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
		throw std::runtime_error("Can not handle file: " + path.toStdString());
	}
	QTextStream in(&f);
	QString text = in.readAll();
	return text;
}

void cutils::truncate_write(const QString& path, const QString& bytes) {
	QFile f(path);
	if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
		throw std::runtime_error("Can not handle file: " + path.toStdString());
	}

	QTextStream out(&f);
	out.setEncoding(QStringConverter::Utf8);
	out << bytes;

	return;
}
