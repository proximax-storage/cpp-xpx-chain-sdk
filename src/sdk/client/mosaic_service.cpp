//
// Created by vulfke on 22.07.19.
//

#include <infrastructure/dto/mosaic_dto.h>
#include <nemcpp/client.h>
#include <sdk/client/mosaic_service.h>

#include <infrastructure/network/http.h>
#include <infrastructure/network/context.h>
#include <infrastructure/dto/multiple_blocks_dto.h>
#include <infrastructure/json/parser.h>

using namespace nem2_sdk;

using nem2_sdk::internal::json::Parser;

MosaicService::MosaicService(
        std::shared_ptr<Config> config,
        std::shared_ptr<internal::network::Context> context,
        internal::network::RequestParamsBuilder builder
) :
        _config(config),
        _context(context),
        _builder(builder)
{}


MosaicInfo MosaicService::getMosaicInfo(MosaicId id) {

    std::stringstream path;

    path << "mosaic/" << id;
    auto requestParams = _builder
            .setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto dto = internal::dto::MosaicInfoDto::from_json(response);
    return dto;
}

std::vector<MosaicInfo> MosaicService::getMosaicInfos(const std::vector<MosaicId>& ids){
    std::string requestJson;
    Parser::Write(ids, requestJson);
    std::stringstream path;
    path << "mosaic/";
    auto requestParams = _builder
            .setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::POST)
            .setRequestBody(requestJson)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto dto = internal::dto::MultipleMosaicDto::from_json(response);
    return dto.mosaics;
}

std::vector<std::string> MosaicService::getMosaicsNames(const std::vector<MosaicId>& ids) {
    std::string requestJson;
    Parser::Write(ids, requestJson);
    std::stringstream path;
    path << "mosaicIds/";
    auto requestParams = _builder
            .setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::POST)
            .setRequestBody(requestJson)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto dto = internal::dto::MosaicNamesDto::from_json(response);
    std::vector<std::string> result;

    for (auto mosaicName : dto.names) {
        result.push_back(mosaicName.name);
    }
    return result;
}
