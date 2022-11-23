
/*
*** Copyright 2022 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include <xpxchaincpp/client/liquidity_provider_service.h>
#include <infrastructure/network/http.h>
#include <infrastructure/network/context.h>
#include <infrastructure/utils/deserialization_json.h>
#include <iostream>
#include <utility>

using namespace xpx_chain_sdk;

using internal::json::dto::from_json;
using internal::json::dto::LiquidityProviderDto;
using internal::json::dto::liquidity_providers_page::LiquidityProvidersPageDto;


LiquidityProviderService::LiquidityProviderService(
	const Config& config,
	std::shared_ptr<internal::network::Context> context
) :
	_config(std::move(config)),
	_context(std::move(context))
{};

LiquidityProvider LiquidityProviderService::getLiquidityProviderByKey(const std::string &key) {
	std::stringstream path;
	path << "liquidity_providers/" << key;

	RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
	return from_json<LiquidityProvider, LiquidityProviderDto>(response);
}

liquidity_providers_page::LiquidityProvidersPage LiquidityProviderService::getLiquidityProviders() {
	std::stringstream path;
	path << "liquidity_providers";

	RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
	return from_json<liquidity_providers_page::LiquidityProvidersPage, LiquidityProvidersPageDto>(response);
}