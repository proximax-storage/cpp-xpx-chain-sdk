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

    class ActiveDataModification {
	public:
		std::string id;
		std::string owner;
		std::string downloadDataCdi;
		uint64_t uploadSize;
	};

    class CompletedDataModification {
	public:
		ActiveDataModification activeDataModification;
		DataModificationState State;
	};

    class ActiveDataModifications {
	public:
		std::vector<ActiveDataModification> active;
	};

	class CompletedDataModifications {
	public:
		std::vector<CompletedDataModification> complete;
	};

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