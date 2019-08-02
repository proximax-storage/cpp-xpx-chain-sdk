
#include "network_service.h"
#include <infrastructure/utils/read_json.h>

using namespace xpx_sdk;


NetworkService::NetworkService(
        std::shared_ptr<Config> config,
        std::shared_ptr<internal::network::Context> context,
        internal::network::RequestParamsBuilder builder
):_config(config), _context(context), _builder(builder){}


NetworkInfo NetworkService::getNetworkInfo() {
    auto requestParams = _builder
            .setPath("network")
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto dto = from_json<NetworkInfo, NetworInfoDto>(response);
    return dto;
}
