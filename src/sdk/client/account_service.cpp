/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include "xpxchaincpp/client/account_service.h"
#include <infrastructure/utils/deserialization_json.h>
#include <infrastructure/json/parser.h>
#include <sstream>
#include <infrastructure/network/http.h>

using namespace xpx_chain_sdk;
using namespace xpx_chain_sdk::internal::json::dto;

using xpx_chain_sdk::internal::json::Parser;
using xpx_chain_sdk::internal::json::dto::from_json;

AccountService::AccountService(
        std::shared_ptr<Config> config,
        std::shared_ptr<internal::network::Context> context)
        : _config(config), _context(context) {}

AccountInfo AccountService::getAccountInfo(const std::string& id) {
    std::stringstream path;
    path << "account/" << id;

    RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto result = from_json<AccountInfo, AccountInfoDto>(response);
    return result;

}

MultipleAccountInfo AccountService::getAccountsInfo(const std::vector<std::string>& ids){
    std::string requestJson;
    Parser::Write(ids, requestJson);
    requestJson = "{\"addresses\":" + requestJson + "}";
    std::string path = "account";

    RequestParamsBuilder builder(_config);
    builder.setPath(path);
    builder.setMethod(internal::network::HTTPRequestMethod::POST);
    builder.setRequestBody(requestJson);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto result = from_json<MultipleAccountInfo, MultipleAccountInfoDto>(response);
    return result;
}

AccountProperties AccountService::getAccountProperties(const std::string& id ){
    std::stringstream path;
    path << "account/" << id << "/properties";

    RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto result = from_json<AccountProperties, AccountPropertyDto>(response);
    return result;
}

MultipleAccountProperty AccountService::getAccountsProperties(const std::vector<std::string>& ids){
    std::string requestJson;
    Parser::Write(ids, requestJson);

    std::string path = "account/properties";

    RequestParamsBuilder builder(_config);
    builder.setPath(path);
    builder.setMethod(internal::network::HTTPRequestMethod::POST);
    builder.setRequestBody(requestJson);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto result = from_json<MultipleAccountProperty, MultipleAccountPropertyDto>(response);
    return result;
}

MultisigInfo AccountService::getMultisigInfo(const std::string& id) {
    std::stringstream path;
    path << "account/" << id << "/multisig";

    RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto result = from_json<MultisigInfo, MultisigInfoDto>(response);
    return result;
}

MultisigGraph AccountService::getMultisigAccountGraphInfo(const std::string& id) {
    std::stringstream path;
    path << "account/" << id << "/multisig/graph";

    RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto result = from_json<MultisigGraph, MultisigGraphDto>(response);
    return result;
}

AccountNames AccountService::getAccountNames(const std::vector<std::string> &addresses) {
    std::string requestJson;
    Parser::Write(addresses, requestJson);
    requestJson = "{\"addresses\":" + requestJson + "}";

    std::string path = "account/names";

    RequestParamsBuilder builder(_config);
    builder.setPath(path);
    builder.setMethod(internal::network::HTTPRequestMethod::POST);
    builder.setRequestBody(requestJson);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto result = from_json<AccountNames, AccountNamesDto>(response);
    return result;
}
