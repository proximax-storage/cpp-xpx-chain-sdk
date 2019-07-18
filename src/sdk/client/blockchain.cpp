#include <nemcpp/client.h>
#include <sdk/client/blockchain_impl.h>
#include <infrastructure/network/http.h>
#include <infrastructure/network/context.h>
#include <infrastructure/dto/block_dto.h>
#include <infrastructure/dto/height.h>
#include <infrastructure/dto/multiple_blocks_dto.h>

using namespace nem2_sdk;

Blockchain::Blockchain(
	std::shared_ptr<Config> config,
	std::shared_ptr<internal::network::Context> context,
	internal::network::RequestParamsBuilder builder
) :
	_config(config),
	_context(context),
	_builder(builder)
{};

uint64_t Blockchain::getBlockchainHeight() {
	auto requestParams = _builder
		.setPath("chain/height")
		.setMethod(internal::network::HTTPRequestMethod::GET)
		.getRequestParams();

	auto result = internal::network::performHTTPRequest(_context, requestParams);

	std::istringstream resultStream(result);
	auto dto = nem2_sdk::internal::dto::HeightDto::from_json(resultStream);
	return dto.height;
}

Block Blockchain::getBlockByHeight(uint64_t height) {
	std::stringstream path;
	path << "block/" << height;

	auto requestParams = _builder
		.setPath(path.str())
		.setMethod(internal::network::HTTPRequestMethod::GET)
		.getRequestParams();

	auto result = internal::network::performHTTPRequest(_context, requestParams);

	std::istringstream resultStream(result);
	return nem2_sdk::internal::dto::BlockDto::from_json(resultStream);
}

std::vector<Block> Blockchain::getBlocksByHeightWithLimit(uint64_t height, uint64_t limit) {
	std::stringstream path;
	path << "blocks/" << height << "/limit/" << limit;

	auto requestParams = _builder
		.setPath(path.str())
		.setMethod(internal::network::HTTPRequestMethod::GET)
		.getRequestParams();

	auto result = internal::network::performHTTPRequest(_context, requestParams);

	std::istringstream resultStream(result);
	return internal::dto::MultipleBlocksDto::from_json(resultStream).blocks;
}
