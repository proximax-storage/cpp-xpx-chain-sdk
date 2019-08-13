/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include "account_service.h"
#include <infrastructure/utils/deserialization_json.h>
#include <infrastructure/json/parser.h>
#include <sstream>

using namespace xpx_sdk;
using namespace xpx_sdk::internal::json::dto;

using xpx_sdk::internal::json::Parser;
using xpx_sdk::internal::json::dto::from_json;

AccountService::AccountService(
        std::shared_ptr<Config> config,
        std::shared_ptr<internal::network::Context> context,
        internal::network::RequestParamsBuilder builder):_config(config), _context(context), _builder(builder) {}

AccountInfo AccountService::getAccountInfo(const std::string& id) {
    std::stringstream path;
    path << "account/" << id;

    auto requestParams = _builder
            .setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    AccountInfo result = from_json<AccountInfo, AccountInfoDto>(response);
    return result;

}

MultipleAccountInfo AccountService::getAccountsInfo(const std::vector<std::string>& ids){
    std::string requestJson;
    Parser::Write(ids, requestJson);
    std::string path = "account/";

    auto requestParams = _builder
            .setPath(path)
            .setMethod(internal::network::HTTPRequestMethod::POST)
            .setRequestBody(requestJson)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto result = from_json<MultipleAccountInfo, MultipleAccountInfoDto>(response);
    return result;

}

AccountProperty AccountService::getAccountProperties(const std::string& id ){
    std::stringstream path;
    path << "account/properties/" << id;

    auto requestParams = _builder
            .setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto result = from_json<AccountProperty, AccountPropertyDto>(response);
    return result;
}

MultipleAccountProperty AccountService::getAccountsProperties(const std::vector<std::string>& ids){
    std::string requestJson;
    Parser::Write(ids, requestJson);

    std::string path = "account/properties";

    auto requestParams = _builder
            .setPath(path)
            .setMethod(internal::network::HTTPRequestMethod::POST)
            .setRequestBody(requestJson)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto result = from_json<MultipleAccountProperty, MultipleAccountPropertyDto>(response);
    return result;
}

MultisigInfo AccountService::getMultisigInfo(const std::string& id) {
    std::stringstream path;
    path << "account/" << id << "/multisig";

    auto requestParams = _builder
            .setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto result = from_json<MultisigInfo, MultisigInfoDto>(response);
    return result;
}

MultisigGraph AccountService::getMultisigAccountGraphInfo(const std::string& id) {
    std::stringstream path;
    path << "account/" << id << "/multisig/graph";

    auto requestParams = _builder
            .setPath(path.str())
            .setMethod(internal::network::HTTPRequestMethod::GET)
            .getRequestParams();

    std::string response = internal::network::performHTTPRequest(_context, requestParams);
    auto result = from_json<MultisigGraph, MultisigGraphDto>(response);
    return result;
}