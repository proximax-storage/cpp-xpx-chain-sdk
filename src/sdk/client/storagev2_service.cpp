/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include "xpxchaincpp/client/storagev2_service.h"
#include <infrastructure/json/parser.h>
#include <sstream>
#include <infrastructure/utils/deserialization_json.h>
#include <infrastructure/network/http.h>

using namespace xpx_chain_sdk;
using namespace xpx_chain_sdk::internal::json::dto;

using xpx_chain_sdk::internal::json::Parser;
using internal::json::dto::from_json;

StorageV2Service::StorageV2Service(
        std::shared_ptr<Config> config,
        std::shared_ptr<internal::network::Context> context)
        : _config(config), _context(context) {}

BcDrive StorageV2Service::getBcDriveByAccountId(const std::string& id){
    std::stringstream path;
    path << "drives_v2/" << id;

    RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto result = from_json<BcDrive, BcDriveDto>(response);
    return result;
}

// BcDrive StorageV2Service::getBcDrives(){
    
// }

// BcDrive StorageV2Service::getBcDriveByOwnerPublicKey(const std::string& id){
//     std::stringstream path;
//     path << "accountv2/" << id << "/drive";

//     RequestParamsBuilder builder(_config);
//     builder.setPath(path.str());
//     builder.setMethod(internal::network::HTTPRequestMethod::GET);

//     std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
//     auto result = from_json<BcDrive, BcDriveDto>(response);
//     return result;
// }

MultipleReplicator StorageV2Service::getReplicators(){
    std::stringstream path;
    path << "replicators_v2";

    RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto result = from_json<MultipleReplicator, MultipleReplicatorDto>(response);
    return result;
}

Replicator StorageV2Service::getReplicatorByPublicKey(const std::string& id){
    std::stringstream path;
    path << "replicators_v2/" << id;

    RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto result = from_json<Replicator, ReplicatorDto>(response);
    return result;
}
