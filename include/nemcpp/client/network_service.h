#pragma once

#include <memory>
#include <nemcpp/config.h>
#include <nemcpp/client/blockchain_service.h>
#include <nemcpp/model/network/network_info.h>

namespace xpx_sdk::internal::network {
	class Context;
	class RequestParamsBuilder;
}

namespace xpx_sdk {

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

