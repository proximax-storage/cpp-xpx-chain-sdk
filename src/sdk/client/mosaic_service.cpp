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

#include <sdk/utils/hex_processing.h>

using namespace xpx_sdk;
using xpx_sdk::internal::json::Parser;
using internal::json::dto::from_json;

using internal::json::dto::MosaicInfoDto;
using internal::json::dto::MosaicNamesDto;
using internal::json::dto::MultipleMosaicInfoDto;

MosaicService::MosaicService(
        std::shared_ptr<Config> config,
        std::shared_ptr<internal::network::Context> context,
        internal::network::RequestParamsBuilder builder
):
        _config(config),
        _context(context),
        _builder(builder)
{}

MosaicInfo MosaicService::getMosaicInfo(const MosaicId& id) {

    std::stringstream path;

    path << "mosaic/" << int_to_hex(id);
    auto requestParams = _builder
            .setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto dto = from_json<MosaicInfo, MosaicInfoDto>(response);
    return dto;
}

MultipleMosaicInfo MosaicService::getMosaicInfos(const std::vector<MosaicId>& ids){
    std::string requestJson;
    std::vector<std::string> hexIds;
    for(auto& mosaicId : ids) {
    	hexIds.push_back(int_to_hex(mosaicId));
    }

    Parser::Write(hexIds, requestJson);
//	std::string tmp;
//	for(int i = 0; i < requestJson.size() ; i++) {
//		if(requestJson[i] != '"') tmp += requestJson[i];
//	}
//	std::swap(requestJson, tmp);
	requestJson = "{\"mosaicIds\":" + requestJson + "}";

    std::cout << "REequest Json: " << requestJson << std::endl;
    std::stringstream path;
    path << "mosaic";
    auto requestParams = _builder
            .setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::POST)
            .setRequestBody(requestJson)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto dto = from_json<MultipleMosaicInfo, MultipleMosaicInfoDto>(response);
    return dto;
}

MosaicNames MosaicService::getMosaicsNames(const std::vector<MosaicId>& ids) {

	std::vector<std::string> hexIds;
	for(auto& mosaicId : ids) {
		hexIds.push_back(int_to_hex(mosaicId));
	}
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
