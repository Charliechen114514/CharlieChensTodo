#include "netimagefetch.h"
#include <QImageReader>

namespace cutils {

bool NetImageFetch::handlable() {
	return true;
}

QImage NetImageFetch::make_components(QNetworkReply& reply) {
	QByteArray imageData = reply.readAll();
	QBuffer buffer(&imageData);
	buffer.open(QIODevice::ReadOnly);

	QImageReader reader(&buffer);
	return reader.read();
}

bool NetPixmapFetch::handlable() {
	return true;
}

QPixmap NetPixmapFetch::make_components(QNetworkReply& reply) {
	QByteArray imageData = reply.readAll();
	QBuffer buffer(&imageData);
	buffer.open(QIODevice::ReadOnly);

	QPixmap pixmap;
	pixmap.loadFromData(buffer.data());
	return pixmap;
}
}
