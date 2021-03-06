/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include <infrastructure/json/dto/mosaic_dto.h>
#include <xpxchaincpp/client.h>
#include <xpxchaincpp/client/mosaic_service.h>

#include <infrastructure/json/dto/multiple_blocks_dto.h>
#include <infrastructure/json/parser.h>

#include <infrastructure/utils/deserialization_json.h>

#include <sdk/utils/hex_processing.h>
#include <infrastructure/network/http.h>

using namespace xpx_chain_sdk;
using xpx_chain_sdk::internal::json::Parser;
using internal::json::dto::from_json;

using internal::json::dto::MosaicInfoDto;
using internal::json::dto::MosaicNamesDto;
using internal::json::dto::MultipleMosaicInfoDto;

MosaicService::MosaicService(
        std::shared_ptr<Config> config,
        std::shared_ptr<internal::network::Context> context,
		std::shared_ptr<RequestParamsBuilder> builder
):
        _config(config),
        _context(context),
        _builder(builder)
{}

MosaicInfo MosaicService::getMosaicInfo(const MosaicId& id) {
    std::stringstream path;
    path << "mosaic/" << int_to_hex(id);

    auto requestParams = _builder
            ->setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto result = from_json<MosaicInfo, MosaicInfoDto>(response);
    return result;
}

MultipleMosaicInfo MosaicService::getMosaicInfos(const std::vector<MosaicId>& ids){

	std::string requestJson;
    std::vector<std::string> hexIds;

    for(auto& mosaicId : ids) {
    	hexIds.push_back(int_to_hex(mosaicId));
    }

    Parser::Write(hexIds, requestJson);
	requestJson = "{\"mosaicIds\":" + requestJson + "}";

    std::string path = "mosaic";

    auto requestParams = _builder
            ->setPath(path)
            .setMethod(internal::network::HTTPRequestMethod::POST)
            .setRequestBody(requestJson)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto result = from_json<MultipleMosaicInfo, MultipleMosaicInfoDto>(response);

    return result;
}

MosaicNames MosaicService::getMosaicsNames(const std::vector<MosaicId>& ids) {
	std::vector<std::string> hexIds;
	for(auto& mosaicId : ids) {
		hexIds.push_back(int_to_hex(mosaicId));
	}

	std::string requestJson;
    Parser::Write(ids, requestJson);

    std::string path = "mosaicIds/";

    auto requestParams = _builder
            ->setPath(path)
            .setMethod(internal::network::HTTPRequestMethod::POST)
            .setRequestBody(requestJson)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto result = from_json<MosaicNames, MosaicNamesDto>(response);
    return result;
}
