/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <memory>
#include <xpxchaincpp/config.h>
#include <xpxchaincpp/client/blockchain_service.h>
#include <xpxchaincpp/model/network/network_config.h>
#include <xpxchaincpp/model/network/network_info.h>
#include <xpxchaincpp/model/network/network_version.h>

namespace xpx_chain_sdk::internal::network {
	class Context;
	class RequestParamsBuilder;
}

namespace xpx_chain_sdk {

	using internal::network::Context;
	using internal::network::RequestParamsBuilder;

	class NetworkService {
    public:
        NetworkService(
                const Config& config,
                std::shared_ptr<internal::network::Context> context);
        ~NetworkService() = default;

        NetworkConfig getNetworkConfig();
        NetworkConfig getNetworkConfigAtHeight(uint64_t height);
        NetworkInfo getNetworkInfo();
        NetworkVersion getNetworkVersion();
        NetworkVersion getNetworkVersionAtHeight(uint64_t height);

    private:
        const Config& _config;
        std::shared_ptr<internal::network::Context> _context;
    };
}

