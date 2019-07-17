#include <nemcpp/client.h>
#include <sdk/client/blockchain_impl.h>
#include <infrastructure/network/http.h>
#include <infrastructure/network/context.h>
#include <infrastructure/dto/block_dto.h>
#include <infrastructure/dto/height.h>

using namespace nem2_sdk;

Blockchain::Blockchain(
	std::shared_ptr<Config> config,
	std::shared_ptr<internal::network::Context> context
) :
	_config(config),
	_context(context)
{};

uint64_t Blockchain::getBlockchainHeight() {
	internal::network::RequestParams requestParams;
	requestParams.path = _config->basePath + "chain/height";
	requestParams.host = _config->nodeAddress;
	requestParams.secure = _config->useSSL;
	requestParams.port = _config->port;
	requestParams.method = internal::network::HTTPRequestMethod::GET;

	auto result = internal::network::performHTTPRequest(_context, requestParams);

	std::istringstream resultStream(result);
	auto dto = nem2_sdk::internal::dto::HeightDto::from_json(resultStream);
	return dto.height;
}

Block Blockchain::getBlockByHeight(uint64_t height) {
	std::stringstream path;
	path << "block/" << height;

	internal::network::RequestParams requestParams;
	requestParams.path = _config->basePath + path.str();
	requestParams.host = _config->nodeAddress;
	requestParams.secure = _config->useSSL;
	requestParams.port = _config->port;
	requestParams.method = internal::network::HTTPRequestMethod::GET;

	auto result = internal::network::performHTTPRequest(_context, requestParams);

	std::istringstream resultStream(result);
	return nem2_sdk::internal::dto::BlockDto::from_json(resultStream);
}
