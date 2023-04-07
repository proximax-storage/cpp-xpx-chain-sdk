/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include "xpxchaincpp/client/namespace_service.h"
#include <infrastructure/json/dto/namespace_dto.h>
#include <infrastructure/json/dto/multiple_namespace_dto.h>

#include <sstream>
#include <sdk/utils/hex_processing.h>
#include <infrastructure/json/parser.h>
#include <infrastructure/utils/deserialization_json.h>
#include <infrastructure/network/http.h>

using namespace xpx_chain_sdk;


using xpx_chain_sdk::internal::json::Parser;
using xpx_chain_sdk::internal::json::dto::from_json;

NamespaceService::NamespaceService(
        const Config& config,
        std::shared_ptr<internal::network::Context> context
) : _config(config), _context(context) {}

NamespaceInfo NamespaceService::getNamespaceInfoById(const NamespaceId& id) {
    std::stringstream path;
    path << "namespace/" << int_to_hex(id);

    RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto result = from_json<NamespaceInfo, internal::json::dto::NamespaceInfoDto>(response);
    return result;
}

NamespaceInfo NamespaceService::getNamespaceInfoByHexId(const std::string& id) {
	std::stringstream path;
	path << "namespace/" << id;

	RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

	std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
	auto result = from_json<NamespaceInfo, internal::json::dto::NamespaceInfoDto>(response);
	return result;
}


MultipleNamespaceInfo NamespaceService::getNamespaceInfoByAccount(const std::string& accountId) {
    std::stringstream path;
    path << "account/" << accountId << "/namespaces";

    RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto result = from_json<MultipleNamespaceInfo, internal::json::dto::MultipleNamespaceDto>(response);
    return result;
}

MultipleNamespaceInfo NamespaceService::getNamespaceInfoByAccounts(const std::vector<std::string>& accountIds){
    std::string requestJson;
    Parser::Write(accountIds, requestJson);
    requestJson = "{\"publicKeys\":" + requestJson + "}";

    std::string path = "account/namespaces";

    RequestParamsBuilder builder(_config);
    builder.setPath(path);
    builder.setMethod(internal::network::HTTPRequestMethod::POST);
    builder.setRequestBody(requestJson);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto namespaceInfos = from_json<MultipleNamespaceInfo, internal::json::dto::MultipleNamespaceDto>(response);
    return namespaceInfos;
}

NamespaceNames NamespaceService::getNamespaceNames(const std::vector<std::string> & namespaceIds) {
    std::string requestJson;
    Parser::Write(namespaceIds, requestJson);
    requestJson = "{\"namespaceIds\":" + requestJson + "}";

    std::string path = "namespace/names";

    RequestParamsBuilder builder(_config);
    builder.setPath(path);
    builder.setMethod(internal::network::HTTPRequestMethod::POST);
    builder.setRequestBody(requestJson);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto result = from_json<NamespaceNames, internal::json::dto::NamespaceNamesDto>(response);
    return result;
}
