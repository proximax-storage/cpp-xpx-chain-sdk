
/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include <xpxchaincpp/client/blockchain_service.h>
#include <infrastructure/network/http.h>
#include <infrastructure/network/context.h>
#include <infrastructure/utils/deserialization_json.h>
#include <xpxchaincpp/model/blockchain/height.h>
#include <iostream>

using namespace xpx_chain_sdk;

using internal::json::dto::from_json;
using internal::json::dto::transactions_from_json;
using internal::json::dto::MultipleBlocksDto;
using internal::json::dto::HeightDto;
using internal::json::dto::BlockDto;
using internal::json::dto::ScoreInfoDto;
using internal::json::dto::StorageInfoDto;


BlockchainService::BlockchainService(
	std::shared_ptr<Config> config,
	std::shared_ptr<internal::network::Context> context
) :
	_config(config),
	_context(context)
{};

uint64_t BlockchainService::getBlockchainHeight() {
    RequestParamsBuilder builder(_config);
    builder.setPath("chain/height");
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
	auto result = from_json<Height, HeightDto>(response);
	return result.height;
}

Block BlockchainService::getBlockByHeight(uint64_t height) {
	std::stringstream path;
	path << "block/" << height;

	RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
	return from_json<Block, BlockDto>(response);
}

MultipleBlock BlockchainService::getBlocksByHeightWithLimit(uint64_t height, uint64_t limit) {
	std::stringstream path;
	path << "blocks/" << height << "/limit/" << limit;

	RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

	std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
	return from_json<MultipleBlock, MultipleBlocksDto>(response);
}

ScoreInfo BlockchainService::getCurrentScore() {
    RequestParamsBuilder builder(_config);
    builder.setPath("chain/score");
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto score = from_json<ScoreInfo,ScoreInfoDto>(response);
    return score;
}

StorageInfo BlockchainService::getStorageInfo() {
    RequestParamsBuilder builder(_config);
    builder.setPath("diagnostic/storage");
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto storageInfo = from_json<StorageInfo, StorageInfoDto>(response);
    return storageInfo;
}