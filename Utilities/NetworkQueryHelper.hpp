#pragma once
#include "cached_proxy.hpp"
#include <QFuture>
#include <QRegularExpression>
#include <QtConcurrent/QtConcurrent>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
namespace cutils {

template <typename FactorizeType>
class NetworkQueryHelper : public CachedProxyPolicy<
                               QFuture<std::optional<FactorizeType>>> {
public:
	using FetchedFuture = QFuture<std::optional<FactorizeType>>;
	virtual ~NetworkQueryHelper() override = default;

	FetchedFuture
	factorized() override final {
		return QtConcurrent::run([this]() -> std::optional<FactorizeType> {
			return make_future();
		});
	}

	void setUrl(const QUrl& pageUrl) {
		handled_url = pageUrl;
	}

	QUrl getUrl() const {
		return handled_url;
	}

	void set_validation(const bool is_valid) {
		need_request = is_valid;
	}

	int getTime_out() const {
		return time_out;
	}
	void setTime_out(int newTime_out) {
		time_out = newTime_out;
	}

protected:
	bool need_request { true };
	QUrl handled_url {};
	int time_out { 30000 };

private:
	virtual FactorizeType make_components(QNetworkReply& reply) = 0;
	std::optional<FactorizeType> make_future() {
		QNetworkAccessManager manager;

		QNetworkRequest req(handled_url);
		req.setRawHeader("User-Agent", "QtApodHtmlFetcher/1.0");
		QNetworkReply* reply = manager.get(req);

		QEventLoop loop;
		QTimer timer;
		timer.setSingleShot(true);

		QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
		QObject::connect(&timer, &QTimer::timeout, &loop, [&loop, reply]() {
			if (reply->isRunning()) {
				qWarning() << "Request timed out, aborting.";
				reply->abort();
			}
			loop.quit();
		});

		timer.start(30000);
		loop.exec();
		qDebug() << handled_url << ": " << reply->error();
		if (reply->error() != QNetworkReply::NoError) {
			reply->deleteLater();
			return std::nullopt; // Ok, not valid
		}

		auto result = make_components(*reply);
		reply->deleteLater();
		return result;
	}
};
}
