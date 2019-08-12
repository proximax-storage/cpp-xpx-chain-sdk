/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include "namespace_service.h"
#include <infrastructure/json/dto/namespace_dto.h>
#include <infrastructure/json/dto/multiple_namespace_dto.h>

#include <sstream>
#include <sdk/utils/hex_processing.h>
#include <infrastructure/json/parser.h>
#include <infrastructure/utils/read_json.h>
using namespace xpx_sdk;


using xpx_sdk::internal::json::Parser;
using xpx_sdk::internal::json::dto::from_json;

NamespaceService::NamespaceService(
        std::shared_ptr<Config> config,
        std::shared_ptr<internal::network::Context> context,
        internal::network::RequestParamsBuilder builder
) : _config(config), _context(context), _builder(builder) {}

NamespaceInfo NamespaceService::getNamespaceInfoById(const NamespaceId& id) {
    std::stringstream path;
    path << "namespace/" << int_to_hex(id);

    auto requestParams = _builder
            .setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto result = from_json<NamespaceInfo, internal::json::dto::NamespaceInfoDto>(response);
    return result;

}

NamespaceInfo NamespaceService::getNamespaceInfoById(const std::string& id) {
	std::stringstream path;
	path << "namespace/" << id;

	auto requestParams = _builder
			.setPath(path.str())
			.setMethod(internal::network::HTTPRequestMethod::GET)
			.getRequestParams();

	std::string response = internal::network::performHTTPRequest(_context, requestParams);
	auto result = from_json<NamespaceInfo, internal::json::dto::NamespaceInfoDto>(response);
	return result;

}


MultipleNamespaceInfo NamespaceService::getNamespaceInfoByAccount(const std::string& accountId) {
    std::stringstream path;
    path << "account/" << accountId << "/namespaces";

    std::cout << path.str() << std::endl;

    auto requestParams = _builder
            .setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto result = from_json<MultipleNamespaceInfo, internal::json::dto::MultipleNamespaceDto>(response);
    return result;
}

MultipleNamespaceInfo NamespaceService::getNamespaceInfoByAccounts(const std::vector<std::string>& accountIds){
    std::string requestJson;
    Parser::Write(accountIds, requestJson);
    std::string path = "account/namespaces";
    auto requestParams = _builder
            .setPath(path)
            .setMethod(internal::network::HTTPRequestMethod::POST)
            .setRequestBody(requestJson)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto namespaceInfos = from_json<MultipleNamespaceInfo, internal::json::dto::MultipleNamespaceDto>(response);
    return namespaceInfos;
}

NamespaceNames NamespaceService::getNamespaceNames(const std::vector<std::string> & namespaceIds) {
    std::string requestJson;
    Parser::Write(namespaceIds, requestJson);
    std::string path = "namespace/names";
    auto requestParams = _builder
            .setPath(path)
            .setMethod(internal::network::HTTPRequestMethod::POST)
            .setRequestBody(requestJson)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto result = from_json<NamespaceNames, internal::json::dto::NamespaceNamesDto>(response);
    return result;
}
