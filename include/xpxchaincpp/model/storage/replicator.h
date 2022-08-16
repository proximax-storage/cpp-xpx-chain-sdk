/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <cstdint>

namespace xpx_chain_sdk {

    class DriveInfo {
    public:
        std::string drive;
        std::string lastApprovedDataModificationId;
        uint64_t initialDownloadWork;
        uint64_t lastCompletedCumulativeDownloadWork;
    };

    class ReplicatorData {
    public:
        std::string key;
        uint32_t version;
        std::vector<DriveInfo> drivesInfo;
        std::vector<std::string> downloadChannels;
    };

	class Replicator {
	public:
        ReplicatorData data;
	};

	class MultipleReplicators {
	public:
		std::vector<Replicator> replicators;
	};
}