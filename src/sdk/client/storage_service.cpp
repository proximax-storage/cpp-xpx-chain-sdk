
/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include <xpxchaincpp/client/storage_service.h>
#include <infrastructure/network/http.h>
#include <infrastructure/network/context.h>
#include <infrastructure/utils/deserialization_json.h>
#include <iostream>
#include <utility>

using namespace xpx_chain_sdk;

using internal::json::dto::from_json;
using internal::json::dto::transactions_from_json;
using internal::json::dto::DriveDto;
using internal::json::dto::MultipleDrivesDto;


StorageService::StorageService(
	std::shared_ptr<Config> config,
	std::shared_ptr<internal::network::Context> context
) :
	_config(std::move(config)),
	_context(std::move(context))
{};

Drive StorageService::getDriveById(const std::string& id) {
	std::stringstream path;
	path << "bcdrive/" << id;

	RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
	return from_json<Drive, DriveDto>(response);
}

MultipleDrives StorageService::getDrives() {
	std::stringstream path;
	path << "bcdrives";

	RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
	return from_json<MultipleDrives, MultipleDrivesDto>(response);
}
