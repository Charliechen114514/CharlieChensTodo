#ifndef NASAAPODFETCHER_H
#define NASAAPODFETCHER_H
#include "NasaAPODData.h"
#include "NetworkQueryHelper.hpp"
#include <QImage>
namespace MainPage {
namespace nasa_apod {

	class NasaAPODFetcher : public cutils::NetworkQueryHelper<NasaAPODData> {

		static constexpr const char* base_url = "https://apod.nasa.gov/apod/ap";

	public:
		NasaAPODFetcher();
		bool handlable() override;

		void setDate(QDate dateForRequest);
		QDate requested_date() const { return request_date; };

	private:
		QDate request_date;
		using cutils::NetworkQueryHelper<NasaAPODData>::setUrl;
		NasaAPODData make_components(QNetworkReply& reply) override;
	};

}
}
#endif // NASAAPODFETCHER_H
