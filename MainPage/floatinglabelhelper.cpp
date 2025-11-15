#include "floatinglabelhelper.h"
#include "CCImageWidget/ccimagewidget.h"
#include "cclabel.h"
#include "helpers/commonwidgetsettingshelper.h"
#include <QEvent>
#include <QVBoxLayout>
#include <qapplication.h>
#include <qscreen.h>
namespace {
QPoint widgetPointToPixmapPoint(CCImageWidget* imageLabel, const QPoint& widgetPt) {
	if (!imageLabel->pixmap() || imageLabel->pixmap().isNull())
		return QPoint(-1, -1);

	const QPixmap pm = imageLabel->pixmap();
	const QSize pixmapSize = pm.size();
	const QSize widgetSize = imageLabel->size();

	const double pw = pixmapSize.width();
	const double ph = pixmapSize.height();
	const double ww = widgetSize.width();
	const double wh = widgetSize.height();

	const double scale = qMin(ww / pw, wh / ph);
	const QSizeF scaledPixmapSize(pw * scale, ph * scale);

	// top-left of pixmap inside label (for centered)
	const double left = (ww - scaledPixmapSize.width()) / 2.0;
	const double top = (wh - scaledPixmapSize.height()) / 2.0;
	const double xInPixmap = (widgetPt.x() - left) / scale;
	const double yInPixmap = (widgetPt.y() - top) / scale;

	const int ix = qBound(0, int(std::round(xInPixmap)), pixmapSize.width() - 1);
	const int iy = qBound(0, int(std::round(yInPixmap)), pixmapSize.height() - 1);
	return QPoint(ix, iy);
}

QColor averageColorAround(const QImage& img, const QPoint& center, int radius = 10, int step = 3) {
	if (center.x() < 0 || center.y() < 0)
		return QColor();

	int x0 = qMax(0, center.x() - radius);
	int x1 = qMin(img.width() - 1, center.x() + radius);
	int y0 = qMax(0, center.y() - radius);
	int y1 = qMin(img.height() - 1, center.y() + radius);

	long long r = 0, g = 0, b = 0;
	long long count = 0;

	for (int y = y0; y <= y1; y += step) {
		for (int x = x0; x <= x1; x += step) {
			QColor c = img.pixelColor(x, y);
			if (c.alpha() == 0)
				continue;
			r += c.red();
			g += c.green();
			b += c.blue();
			++count;
		}
	}

	if (count == 0)
		return QColor();
	return QColor(int(r / count), int(g / count), int(b / count));
}

inline QColor bestTextColorForBackground(const QColor& bg) {
	double L = 0.2126 * bg.redF() + 0.7152 * bg.greenF() + 0.0722 * bg.blueF();
	return L < 0.5 ? Qt::white : Qt::black;
}

}

FloatingLabelHelper::FloatingLabelHelper(QWidget* parent)
    : QWidget { parent, Qt::ToolTip | Qt::WindowStaysOnTopHint } {
	setAttribute(Qt::WA_ShowWithoutActivating);
	setAttribute(Qt::WA_TransparentForMouseEvents, true);
	setWindowModality(Qt::NonModal);
	setFocusPolicy(Qt::NoFocus);

	label = new CCLabel(this);
	label->setAlignment(Qt::AlignCenter);

	CCWidgetLibrary::WidgetHelpers::setWidgetBackgroundTransparent(label);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(label);
	layout->setContentsMargins(0, 0, 0, 0);

	hide();
}

void FloatingLabelHelper::setTitleText(const QString& text) {
	label->setText(text);
}

void FloatingLabelHelper::displayBusyText() {
	label->setText("Waiting for loading...");
}

void FloatingLabelHelper::auto_adjust_text_color(CCImageWidget* cover_widget) {
	if (!cover_widget || !cover_widget->pixmap() || cover_widget->pixmap().isNull())
		return;
	QPoint globalCenter = mapToGlobal(rect().center());
	QPoint coverLocal = cover_widget->mapFromGlobal(globalCenter); // cover_widget 坐标系内的点

	QPoint pixPt = widgetPointToPixmapPoint(cover_widget, coverLocal);
	if (pixPt.x() < 0)
		return;

	QImage img = cover_widget->pixmap().toImage().convertToFormat(QImage::Format_ARGB32);

	QColor avg = averageColorAround(img, pixPt, /*radius=*/12, /*step=*/3);
	if (!avg.isValid()) {
		avg = img.pixelColor(qBound(0, pixPt.x(), img.width() - 1), qBound(0, pixPt.y(), img.height() - 1));
	}

	QColor text = bestTextColorForBackground(avg);
	label->setTextColor(text);
}

void FloatingLabelHelper::setTextColor(const QColor& c) {
	label->setTextColor(c);
}

void FloatingLabelHelper::setFontSize(int fontSize) {
	auto f = label->font();
	f.setPointSize(fontSize);
	label->setFont(f);
}
