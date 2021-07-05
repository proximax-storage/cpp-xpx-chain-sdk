/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include "xpxchaincpp/client/network_service.h"
#include <infrastructure/utils/deserialization_json.h>
#include <infrastructure/network/http.h>

using namespace xpx_chain_sdk;

using xpx_chain_sdk::internal::json::dto::NetworkConfigDto;
using xpx_chain_sdk::internal::json::dto::NetworkInfoDto;
using xpx_chain_sdk::internal::json::dto::NetworkVersionDto;
using xpx_chain_sdk::internal::json::dto::HeightDto;
using xpx_chain_sdk::internal::json::dto::from_json;

static uint64_t getBlockchainHeight(
        std::shared_ptr<internal::network::Context> context,
        std::shared_ptr<Config> _config) {
    RequestParamsBuilder builder(_config);
    builder.setPath("chain/height");
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(context, builder.getRequestParams());
    auto result = from_json<Height, HeightDto>(response);
    return result.height;
}

NetworkService::NetworkService(
        std::shared_ptr<Config> config,
        std::shared_ptr<internal::network::Context> context
):_config(config), _context(context) {}


NetworkInfo NetworkService::getNetworkInfo() {
    RequestParamsBuilder builder(_config);
    builder.setPath("network");
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto result = from_json<NetworkInfo, NetworkInfoDto>(response);
    return result;
}

NetworkConfig NetworkService::getNetworkConfig() {
    uint64_t height = getBlockchainHeight(_context, _config);
    return getNetworkConfigAtHeight(height);
}

NetworkConfig NetworkService::getNetworkConfigAtHeight(uint64_t height) {
    std::stringstream path;
    path << "config/" << height;

    RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto result = from_json<NetworkConfig, NetworkConfigDto>(response);
    return result;
}

NetworkVersion NetworkService::getNetworkVersion() {
    uint64_t height = getBlockchainHeight(_context, _config);
    return getNetworkVersionAtHeight(height);
}

NetworkVersion NetworkService::getNetworkVersionAtHeight(uint64_t height) {
    std::stringstream path;
    path << "upgrade/" << height;

    RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto result = from_json<NetworkVersion, NetworkVersionDto>(response);
    return result;
}
