#include <memory>

#include <nemcpp/client.h>
#include <sdk/client/blockchain_service.h>
#include <infrastructure/network/http.h>
#include <infrastructure/network/context.h>
#include <infrastructure/dto/block_dto.h>
#include <infrastructure/dto/height.h>
#include <infrastructure/dto/multiple_blocks_dto.h>

using namespace nem2_sdk;

BlockchainService::BlockchainService(
	std::shared_ptr<Config> config,
	std::shared_ptr<internal::network::Context> context,
	internal::network::RequestParamsBuilder builder
) :
	_config(config),
	_context(context),
	_builder(builder)
{};

uint64_t BlockchainService::getBlockchainHeight() {
	auto requestParams = _builder
		.setPath("chain/height")
		.setMethod(internal::network::HTTPRequestMethod::GET)
		.getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
	auto dto = nem2_sdk::internal::dto::HeightDto::from_json(response);
	return dto.height;
}

Block BlockchainService::getBlockByHeight(uint64_t height) {
	std::stringstream path;
	path << "data/" << height;

	auto requestParams = _builder
		.setPath(path.str())
		.setMethod(internal::network::HTTPRequestMethod::GET)
		.getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
	return nem2_sdk::internal::dto::BlockDto::from_json(response);
}

std::vector<Block> BlockchainService::getBlocksByHeightWithLimit(uint64_t height, uint64_t limit) {
	std::stringstream path;
	path << "blocks/" << height << "/limit/" << limit;

	auto requestParams = _builder
		.setPath(path.str())
		.setMethod(internal::network::HTTPRequestMethod::GET)
		.getRequestParams();

	std::string response = internal::network::performHTTPRequest(_context, requestParams);
	return internal::dto::MultipleBlocksDto::from_json(response).blocks;
}

ScoreInfo BlockchainService::getCurrentScore() {
    auto requestParams = _builder
            .setPath("chain/score")
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto score = nem2_sdk::internal::dto::ScoreInfoDto::from_json(response);
    return score;
}

StorageInfo BlockchainService::getStorageInfo() {
    auto requestParams = _builder
            .setPath("diagnostic/storage")
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto storageInfo = nem2_sdk::internal::dto::StorageInfoDto::from_json(response);
    return storageInfo;
}