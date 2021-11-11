/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/types.h>
#include <string>
#include <map>

namespace xpx_chain_sdk {
    enum class DataModificationState : uint8_t {
		/// Data modification has been approved.
		Succeeded,

		/// Data modification has been cancelled.
		Cancelled
	};

    struct ActiveDataModification {
		/// Id of data modification.
		std::string id;

		/// Public key of the drive owner.
		std::string owner;

		/// CDI of download data.
		std::string downloadDataCdi;

		/// Upload size of data.
		uint64_t uploadSize;
	};

    struct CompletedDataModification : ActiveDataModification {
		CompletedDataModification(const ActiveDataModification& modification, DataModificationState state)
			: ActiveDataModification(modification)
			, State(state)
		{}

		/// Completion state.
		DataModificationState State;
	};

    using ActiveDataModifications = std::vector<ActiveDataModification>;
    using CompletedDataModifications = std::vector<CompletedDataModification>;

    class BcDrive {
    public:
        std::string bcDriveAccount;
        std::string owner;
		std::string rootHash;
		uint64_t size;
		uint64_t usedSize;
		uint64_t metaFilesSize;
		uint16_t replicatorCount;
		ActiveDataModifications activeDataModifications;
		CompletedDataModifications completedDataModifications;
    };
}