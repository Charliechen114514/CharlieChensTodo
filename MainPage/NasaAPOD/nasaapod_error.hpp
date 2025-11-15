#ifndef NASAAPOD_ERROR_HPP
#define NASAAPOD_ERROR_HPP
#include <stdexcept>
namespace MainPage {
namespace nasa_apod {
#define BASE_DECLEAR_ERROR_TYPE(ClassType)        \
	class ClassType : public std::runtime_error { \
	public:                                       \
		using std::runtime_error::runtime_error;  \
	}
}

BASE_DECLEAR_ERROR_TYPE(APODDataRequestFailed);

#undef BASE_DECLEAR_ERROR_TYPE

}

#endif // NASAAPOD_ERROR_HPP
