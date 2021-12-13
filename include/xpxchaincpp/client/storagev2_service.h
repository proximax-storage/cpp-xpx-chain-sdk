/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/model/transaction_simple/transaction_container.h>
#include <xpxchaincpp/model/storagev2/bcdrive_info.h>
#include <xpxchaincpp/model/storagev2/replicator_info.h>

namespace xpx_chain_sdk::internal::network {
	class Context;
	class RequestParamsBuilder;
}

namespace xpx_chain_sdk {

	using internal::network::Context;
	using internal::network::RequestParamsBuilder;
	using transactions_info::TransactionContainer;

    class StorageV2Service {
    public:
        StorageV2Service(
				std::shared_ptr<Config> config,
				std::shared_ptr<internal::network::Context> context);
		~StorageV2Service() = default;

        // BcDrive getBcDrives();
        BcDrive getBcDriveByAccountId(const std::string& id);
        // BcDrive getBcDriveByOwnerPublicKey(const std::string& id);
        // Replicator getReplicators();
        Replicator getReplicatorByPublicKey(const std::string& id);
        Replicator getReplicatorByBlsKey(const std::string& id);

    private:
        std::shared_ptr<Config> _config;
        std::shared_ptr<internal::network::Context> _context;
    };
}


