#pragma once

#include "Utilities_global.h"
#include "cached_policy_error.hpp"
#include <memory>

namespace cutils {

template <class ProxiedObject>
class CachedProxyPolicy {
public:
	virtual ~CachedProxyPolicy() = default;
	virtual ProxiedObject factorized() = 0;
	virtual bool handlable() = 0;
};

template <class ProxiedObject, int cached_policy_level = 3>
class CachedProxy {
public:
	virtual ~CachedProxy() = default;
	bool register_policy(
	    CachedProxyPolicy<ProxiedObject>* policy,
	    const int level,
	    const bool allow_duplicate = false) {
		// 0. check the params
		if (level >= cached_policy_level) {
			throw ProxyLevelInvalid<cached_policy_level>(level);
		}

		if (!policy) {
			throw CachedProxyEmpty("policy provided empty!");
		}

		// 1. check if the policy is registered in level already
		if (policies_level[level] && !allow_duplicate) {
			throw DuplicateProxyRegistrytion("Proxy in level registered!");
		}

		// OK then we registered direct
		policies_level[level] = std::unique_ptr<CachedProxyPolicy<ProxiedObject>>(policy);
	}

	bool remove_policy(CachedProxyPolicy<ProxiedObject>* policy) {
		for (const auto& each : policies_level) {
			if (each == policy) {
				each.reset(nullptr);
				return true;
			}
		}

		return false;
	}

	void remove_policy(const int level) {
		policies_level[level].reset(nullptr);
	}

	std::unique_ptr<ProxiedObject> factorizedOne() {
		for (const auto& each : policies_level) {
			if (each.handlable()) {
				auto result = each.factorized();
				if (result) {
					// OK, result finished
					return result.value();
				}
			}
		}

		return {};
	}

private:
	std::array<std::unique_ptr<
	               CachedProxyPolicy<ProxiedObject>>,
	           cached_policy_level>
	    policies_level {};
};

} // end namespace cutils
