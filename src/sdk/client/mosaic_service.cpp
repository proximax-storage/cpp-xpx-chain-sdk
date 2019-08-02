/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include <infrastructure/json/dto/mosaic_dto.h>
#include <nemcpp/client.h>
#include <sdk/client/mosaic_service.h>

#include <infrastructure/json/dto/multiple_blocks_dto.h>
#include <infrastructure/json/parser.h>

#include <infrastructure/utils/read_json.h>

using namespace xpx_sdk;
using xpx_sdk::internal::json::Parser;
using internal::json::dto::from_json;

using internal::json::dto::MosaicInfoDto;
using internal::json::dto::MosaicNamesDto;
using internal::json::dto::MultipleMosaicDto;

MosaicService::MosaicService(
        std::shared_ptr<Config> config,
        std::shared_ptr<internal::network::Context> context,
        internal::network::RequestParamsBuilder builder
):
        _config(config),
        _context(context),
        _builder(builder)
{}

MosaicInfo MosaicService::getMosaicInfo(const std::string& id) {

    std::stringstream path;

    path << "mosaic/" << id;
    auto requestParams = _builder
            .setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto dto = from_json<MosaicInfo, MosaicInfoDto>(response);
    return dto;
}

MultipleMosaicInfo MosaicService::getMosaicInfos(const std::vector<std::string>& ids){
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
    auto dto = from_json<MultipleMosaicInfo, MultipleMosaicDto>(response);
    return dto;
}

MosaicNames MosaicService::getMosaicsNames(const std::vector<std::string>& ids) {
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
    auto dto = from_json<MosaicNames, MosaicNamesDto>(response);

    return dto;
}
