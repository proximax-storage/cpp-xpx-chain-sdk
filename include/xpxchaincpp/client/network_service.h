#pragma once

#include <memory>
#include <xpxchaincpp/config.h>
#include <xpxchaincpp/client/blockchain_service.h>
#include <xpxchaincpp/model/network/network_info.h>

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
                std::shared_ptr<Config> config,
                std::shared_ptr<internal::network::Context> context,
				std::shared_ptr<RequestParamsBuilder> builder
        );
        ~NetworkService() = default;
        NetworkInfo getNetworkInfo();
    private:
        std::shared_ptr<Config> _config;
        std::shared_ptr<internal::network::Context> _context;
		std::shared_ptr<RequestParamsBuilder> _builder;
    };
}

