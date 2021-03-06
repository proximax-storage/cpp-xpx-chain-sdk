
/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include <xpxchaincpp/client.h>
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
	std::shared_ptr<internal::network::Context> context,
	std::shared_ptr<RequestParamsBuilder>  builder
) :
	_config(config),
	_context(context),
	_builder(builder)
{};

uint64_t BlockchainService::getBlockchainHeight() {
	auto requestParams = _builder
		-> setPath("chain/height")
		.setMethod(internal::network::HTTPRequestMethod::GET)
		.getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
	auto result = from_json<Height, HeightDto>(response);
	return result.height;
}

Block BlockchainService::getBlockByHeight(uint64_t height) {
	std::stringstream path;
	path << "block/" << height;

	auto requestParams = _builder
		->setPath(path.str())
		.setMethod(internal::network::HTTPRequestMethod::GET)
		.getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
	return from_json<Block, BlockDto>(response);
}

MultipleBlock BlockchainService::getBlocksByHeightWithLimit(uint64_t height, uint64_t limit) {
	std::stringstream path;
	path << "blocks/" << height << "/limit/" << limit;

	auto requestParams = _builder
		->setPath(path.str())
		.setMethod(internal::network::HTTPRequestMethod::GET)
		.getRequestParams();

	std::string response = internal::network::performHTTPRequest(_context, requestParams);
	return from_json<MultipleBlock, MultipleBlocksDto>(response);
}

transactions_info::TransactionContainer BlockchainService::getBlockTransactions(uint64_t height) {
	std::stringstream path;
	path << "blocks/" << height << "/transactions";

	auto requestParams = _builder
			->setPath(path.str())
			.setMethod(internal::network::HTTPRequestMethod::GET)
			.getRequestParams();

	std::string response = internal::network::performHTTPRequest(_context, requestParams);
	return transactions_from_json(response);


}

ScoreInfo BlockchainService::getCurrentScore() {
    auto requestParams = _builder
            ->setPath("chain/score")
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto score = from_json<ScoreInfo,ScoreInfoDto>(response);
    return score;
}

StorageInfo BlockchainService::getStorageInfo() {
    auto requestParams = _builder
            ->setPath("diagnostic/storage")
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto storageInfo = from_json<StorageInfo, StorageInfoDto>(response);
    return storageInfo;
}