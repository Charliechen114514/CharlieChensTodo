#ifndef NETIMAGEFETCH_H
#define NETIMAGEFETCH_H
#include "NetworkQueryHelper.hpp"
#include "Utilities_global.h"
#include <QImage>
#include <QPixmap>

namespace cutils {
class UTILITIES_EXPORT NetImageFetch : public NetworkQueryHelper<QImage> {
public:
	bool handlable() override;
	~NetImageFetch() override = default;

private:
	QImage make_components(QNetworkReply& reply) override;
};

class UTILITIES_EXPORT NetPixmapFetch : public NetworkQueryHelper<QPixmap> {
public:
	bool handlable() override;
	~NetPixmapFetch() override = default;

private:
	QPixmap make_components(QNetworkReply& reply) override;
};
}

#endif // NETIMAGEFETCH_H
