
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
using internal::json::dto::drives_page::DrivesPageDto;
using internal::json::dto::DownloadChannelDto;
using internal::json::dto::download_channels_page::DownloadChannelsPageDto;
using internal::json::dto::ReplicatorDto;
using internal::json::dto::replicators_page::ReplicatorsPageDto;


StorageService::StorageService(
	std::shared_ptr<Config> config,
	std::shared_ptr<internal::network::Context> context
) :
	_config(std::move(config)),
	_context(std::move(context))
{};

Drive StorageService::getDriveById(const std::string& id) {
	std::stringstream path;
	path << "bcdrives/" << id;

	RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
	return from_json<Drive, DriveDto>(response);
}

drives_page::DrivesPage StorageService::getDrives(const DrivesPageOptions& options) {
	std::stringstream path;
	path << "bcdrives";

	RequestParamsBuilder builder(_config);
    builder.setPath(path.str(), options.toMap());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
	return from_json<drives_page::DrivesPage, DrivesPageDto>(response);
}

Replicator StorageService::getReplicatorById(const std::string &id) {
    std::stringstream path;
    path << "replicators/" << id;

    RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    return from_json<Replicator, ReplicatorDto>(response);
}

replicators_page::ReplicatorsPage StorageService::getReplicators(const ReplicatorsPageOptions& options) {
    std::stringstream path;
    path << "replicators";

    RequestParamsBuilder builder(_config);
    builder.setPath(path.str(), options.toMap());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    return from_json<replicators_page::ReplicatorsPage, ReplicatorsPageDto>(response);
}

DownloadChannel StorageService::getDownloadChannelById(const std::string &id) {
    std::stringstream path;
    path << "download_channels/" << id;

    RequestParamsBuilder builder(_config);
    builder.setPath(path.str());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    return from_json<DownloadChannel, DownloadChannelDto>(response);
}

download_channels_page::DownloadChannelsPage StorageService::getDownloadChannels(const DownloadChannelsPageOptions& options) {
    std::stringstream path;
    path << "download_channels";

    RequestParamsBuilder builder(_config);
    builder.setPath(path.str(), options.toMap());
    builder.setMethod(internal::network::HTTPRequestMethod::GET);

    std::string response = internal::network::performHTTPRequest(_context, builder.getRequestParams());
    return from_json<download_channels_page::DownloadChannelsPage, DownloadChannelsPageDto>(response);
}