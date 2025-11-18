#include "nasaapodfetcher.h"
#include "nasaapod_error.hpp"
#include "netimagefetch.h"

namespace {

QString get_url(const char* base, const QDate& date) {
	return base + date.toString("yyMMdd") + ".html";
}

}

namespace MainPage::nasa_apod {
NasaAPODFetcher::NasaAPODFetcher() {
	setDate(QDate::currentDate());
}

bool NasaAPODFetcher::handlable() {
	return true;
}

void NasaAPODFetcher::setDate(QDate dateForRequest) {
	request_date = dateForRequest;
	setUrl(get_url(base_url, dateForRequest));
}

NasaAPODData NasaAPODFetcher::make_components(QNetworkReply& reply) {
	QString html = QString::fromUtf8(reply.readAll());

	// 1. Image
	static QRegularExpression imgRe("<img[^>]*\\s+src=\"([^\"]+)\"[^>]*>",
	                                QRegularExpression::CaseInsensitiveOption);

	QRegularExpressionMatch imgMatch = imgRe.match(html);
	QString mainImageUrl;
	if (imgMatch.hasMatch()) {
		mainImageUrl = imgMatch.captured(1);
	}

	// 2. Title
	static QRegularExpression titleRe(R"(<b>\s*(?<title>[^<]+)\s*</b>\s*<br>)",
	                                  QRegularExpression::CaseInsensitiveOption);
	static QRegularExpressionMatch titleMatch = titleRe.match(html);
	QString title = titleMatch.hasMatch() ? titleMatch.captured("title").trimmed() : QString("<no title>");

	// 3. Explanation
	static QRegularExpression explRe(R"(<b>\s*Explanation:\s*</b>(?<expl>.*?)</center>)",
	                                 QRegularExpression::DotMatchesEverythingOption | QRegularExpression::CaseInsensitiveOption);
	static QRegularExpressionMatch explMatch = explRe.match(html);
	QString explanation;
	if (explMatch.hasMatch()) {
		explanation = explMatch.captured("expl").trimmed();
		static QRegularExpression _("<[^>]+>");
		// remove html title
		explanation.replace(_, "");
	} else {
		explanation = QString("<no explanation found>");
	}

	int pos = explanation.indexOf("Tomorrow's picture");
	if (pos != -1)
		explanation = explanation.left(pos);
	static auto clean_slash = QRegularExpression("\\s*\\n\\s*");
	static auto clean_space = QRegularExpression("\\s{2,}");
	explanation.replace(clean_slash, "\n");
	explanation.replace(clean_space, " ");
	explanation = explanation.trimmed();
	auto image_fetcher = new cutils::NetPixmapFetch;

	QUrl resolved = handled_url.resolved({ mainImageUrl });
	image_fetcher->setUrl(resolved);

	try {
		auto image_future = image_fetcher->factorized().result();
		if (!image_future) {
			throw APODDataRequestFailed("Image Request Failed!");
		}

		QPixmap image_result = image_future.value();
		if (image_result.isNull()) {
			throw APODDataRequestFailed("Image Request Success, but the image is invalid!");
		}

		NasaAPODData data;
		data.image_url = mainImageUrl;
		data.image = std::move(image_result);
		data.descriptions = explanation;
		data.title = title;

		return data;
	} catch (const std::exception& e) {
		qWarning() << "APOD make_components exception:" << e.what();
		return {};
	} catch (...) {
		qWarning() << "APOD make_components unknown exception";
		return {};
	}
}
}
