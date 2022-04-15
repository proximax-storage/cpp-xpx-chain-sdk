/*
*** Copyright 2022 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <xpxchaincpp/config.h>
#include <xpxchaincpp/model/liquidity_provider/liquidity_providers_page.h>
#include <memory>


namespace xpx_chain_sdk::internal::network {
	class Context;
	class RequestParamsBuilder;
}

namespace xpx_chain_sdk {

	using internal::network::Context;
	using internal::network::RequestParamsBuilder;

	class LiquidityProviderService {
	public:
        LiquidityProviderService(
                std::shared_ptr<Config> config,
                std::shared_ptr<internal::network::Context> context);
		~LiquidityProviderService() = default;

        LiquidityProvider getLiquidityProviderByKey(const std::string& key);
        liquidity_providers_page::LiquidityProvidersPage getLiquidityProviders();

	private:
		std::shared_ptr<Config> _config;
		std::shared_ptr<Context> _context;
	};
}
