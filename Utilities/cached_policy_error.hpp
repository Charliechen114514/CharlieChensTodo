#ifndef CACHED_POLICY_ERROR_HPP
#define CACHED_POLICY_ERROR_HPP
#include "Utilities_global.h"
#include <stdexcept>
#include <string>
namespace cutils {

#define BASE_DECLEAR_ERROR_TYPE(ClassType)                         \
	class UTILITIES_EXPORT ClassType : public std::runtime_error { \
	public:                                                        \
		using std::runtime_error::runtime_error;                   \
	}

BASE_DECLEAR_ERROR_TYPE(CachedProxyEmpty);
BASE_DECLEAR_ERROR_TYPE(DuplicateProxyRegistrytion);

template <int max_level>
class UTILITIES_EXPORT ProxyLevelInvalid : std::exception {
public:
	ProxyLevelInvalid(const int indicated_level)
	    : msg(
	          "Given the level " + std::to_string(indicated_level) + "But The Ok level between 0 and " + std::to_string(max_level - 1)) {
	}

	char const* what() const override {
		return msg.c_str();
	}

private:
	std::string msg;
};

#undef BASE_DECLEAR_ERROR_TYPE // Dont Leak out the defines
}

#endif // CACHED_POLICY_ERROR_HPP
