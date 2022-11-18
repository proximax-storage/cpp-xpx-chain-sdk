/*
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <xpxchaincpp/config.h>
#include <xpxchaincpp/model/storage/drives_page.h>
#include <xpxchaincpp/model/storage/download_channels_page.h>
#include <xpxchaincpp/model/storage/replicators_page.h>
#include "xpxchaincpp/model/storage/download_channels_page_options.h"
#include "xpxchaincpp/model/storage/replicators_page_options.h"
#include "xpxchaincpp/model/storage/drives_page_options.h"
#include <memory>


namespace xpx_chain_sdk::internal::network {
	class Context;
	class RequestParamsBuilder;
}

namespace xpx_chain_sdk {

	using internal::network::Context;
	using internal::network::RequestParamsBuilder;

	class StorageService {
	public:
		StorageService(
				std::shared_ptr<Config> config,
				std::shared_ptr<internal::network::Context> context);
		~StorageService() = default;

		Drive getDriveById(const std::string& id);
        drives_page::DrivesPage getDrives(const DrivesPageOptions& options);
        Replicator getReplicatorById(const std::string& id);
        replicators_page::ReplicatorsPage getReplicators(const ReplicatorsPageOptions& options);
        DownloadChannel getDownloadChannelById(const std::string& id);
        download_channels_page::DownloadChannelsPage getDownloadChannels(const DownloadChannelsPageOptions& options);

	private:
		std::shared_ptr<Config> _config;
		std::shared_ptr<Context> _context;
	};
}
