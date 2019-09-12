
#include "xpxchaincpp/client/network_service.h"
#include <infrastructure/utils/deserialization_json.h>
#include <infrastructure/network/http.h>

using namespace xpx_chain_sdk;

using xpx_chain_sdk::internal::json::dto::NetworkInfoDto;
using xpx_chain_sdk::internal::json::dto::from_json;


NetworkService::NetworkService(
        std::shared_ptr<Config> config,
        std::shared_ptr<internal::network::Context> context,
		std::shared_ptr<RequestParamsBuilder> builder
):_config(config), _context(context), _builder(builder){}


NetworkInfo NetworkService::getNetworkInfo() {
    auto requestParams = _builder
            ->setPath("network")
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto result = from_json<NetworkInfo, NetworkInfoDto>(response);
    return result;
}
