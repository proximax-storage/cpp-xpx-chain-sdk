/*
*** Copyright 2023 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include <infrastructure/json/dto/supercontract_v2_dto.h>
#include <xpxchaincpp/client/supercontract_v2_service.h>

#include <infrastructure/json/parser.h>
#include <infrastructure/utils/deserialization_json.h>

#include <sdk/utils/hex_processing.h>
#include <infrastructure/network/http.h>

using namespace xpx_chain_sdk;
using namespace xpx_chain_sdk::internal::json::dto;

using xpx_chain_sdk::internal::json::Parser;
using xpx_chain_sdk::internal::json::dto::from_json;

SuperContractV2Service::SuperContractV2Service(
        std::shared_ptr<Config> config,
        std::shared_ptr<internal::network::Context> context
) :
        _config(config),
        _context(context)
{}

SuperContractInfo SuperContractV2Service::getSuperContractByKey(const std::string& contractKey) {
    std::stringstream path;
    path << "supercontracts/" << contractKey;

    RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    auto result = from_json<SuperContractInfo, SuperContractInfoDto>(response);
    return result;
}
