#include <nemcpp/client.h>
#include <nemcpp/client/block.h>
#include <infrastructure/network/http.h>
#include <infrastructure/network/context.h>
#include <infrastructure/dto/lower_higher.h>

using namespace nem2_sdk;

Blockchain::Blockchain(
	std::shared_ptr<Config> config
) :
	_config(config)
{};

uint64_t Blockchain::getBlockchainHeight() {
	internal::network::RequestParams requestParams;
	requestParams.path = _config->basePath + "chain/height";
	requestParams.host = _config->nodeAddress;
	requestParams.secure = _config->useSSL;
	requestParams.port = _config->port;
	requestParams.method = internal::network::HTTPRequestMethod::GET;

	auto context = std::make_shared<internal::network::Context>();
	auto result = internal::network::performHTTPRequest(context, requestParams);

	std::istringstream resultStream(result);
	auto dto = nem2_sdk::internal::dto::LowerHigherDto::from_json(resultStream);
	return dto.higher;
}
