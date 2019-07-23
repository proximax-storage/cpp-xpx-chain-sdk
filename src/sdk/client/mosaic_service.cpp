//
// Created by vulfke on 22.07.19.
//

#include <infrastructure/dto/mosaic_dto.h>
#include <nemcpp/client.h>
#include <sdk/client/mosaic_service.h>

#include <infrastructure/network/http.h>
#include <infrastructure/network/context.h>
#include <infrastructure/dto/multiple_blocks_dto.h>

using namespace nem2_sdk;

MosaicService::MosaicService(
        std::shared_ptr<Config> config,
        std::shared_ptr<internal::network::Context> context,
        internal::network::RequestParamsBuilder builder
) :
        _config(config),
        _context(context),
        _builder(builder)
{}


MosaicData MosaicService::getMosaicInfo(MosaicId id) {

    std::stringstream path;

    path << "mosaic/" << id;
    auto requestParams = _builder
            .setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto dto = internal::dto::MosaicDto::from_json(response);
    return dto.data;
}

std::vector<MosaicData> MosaicService::getMosaicInfos(const std::vector<MosaicId>& ids){
    std::vector<MosaicData> result;
    return result;
}

std::vector<std::string> MosaicService::getMosaicsNames(const std::vector<MosaicId>& ids) {
    std::vector<std::string> result;
    return result;
}
