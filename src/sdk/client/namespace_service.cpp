/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include "namespace_service.h"
#include <infrastructure/dto/namespace_dto.h>
#include <infrastructure/dto/multiple_namespace_dto.h>

#include <sstream>
#include <sdk/utils/hex_processing.h>
#include <infrastructure/json/parser.h>
using namespace nem2_sdk;


using nem2_sdk::internal::json::Parser;

NamespaceInfo NamespaceService::getNamespaceInfoById(const NamespaceId& id) {
    std::stringstream path;
    path << "namespace/" << int_to_hex(id);

    auto requestParams = _builder
            .setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto dto = internal::dto::NamespaceInfoDto::from_json(response);
    return dto;

}

std::vector<NamespaceInfo> NamespaceService::getNamespaceInfoByAccount(const std::string& accountId) {
    std::stringstream path;
    path << "account/" << accountId << "/namespace";

    auto requestParams = _builder
            .setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto dto = internal::dto::MultipleNamespaceDto::from_json(response);
    return dto;
}

std::vector<NamespaceInfo> NamespaceService::getNamespaceInfoByAccounts(const std::vector<std::string>& accountIds){
    std::string requestJson;
    Parser::Write(accountIds, requestJson);
    std::stringstream path;
    path << "account/namespaces";
    auto requestParams = _builder
            .setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::POST)
            .setRequestBody(requestJson)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto namespaceInfos = internal::dto::MultipleNamespaceDto::from_json(response);
    return namespaceInfos;
}

std::vector<std::string> NamespaceService::getNamespaceNames(const std::vector<std::string> & namespaceIds) {
    std::string requestJson;
    Parser::Write(namespaceIds, requestJson);
    std::stringstream path;
    path << "namespace/names";
    auto requestParams = _builder
            .setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::POST)
            .setRequestBody(requestJson)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto dto = internal::dto::NamespaceNamesDto::from_json(response);
    return dto;
}
