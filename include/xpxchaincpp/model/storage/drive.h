/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <cstdint>

namespace xpx_chain_sdk {

    class DataModification {
    public:
        std::string id;
        std::string owner;
        std::string downloadDataCdi;
        uint64_t expectedUploadSize;
        uint64_t actualUploadSize;
        std::string folderName;
        bool readyForApproval;
    };

    class ActiveDataModification {
    public:
        DataModification  dataModification;
    };

    class CompletedDataModification {
    public:
        DataModification dataModification;
        uint8_t modificationState;
    };

    class ConfirmedUsedSize {
    public:
        std::string replicator;
        uint64_t size;
    };

    class Shard {
    public:
        std::string downloadChannelId;
        std::vector<std::string> replicators;
    };

    class Verification {
    public:
        std::string verificationTrigger;
        uint64_t expiration;
        bool expired;
        std::vector<Shard> shards;
    };

    class DriveData {
    public:
        std::string multisig;
        std::string multisigAddress;
        std::string owner;
        std::string rootHash;
        uint64_t size;
        uint64_t usedSize;
        uint64_t metaFilesSize;
        uint16_t replicatorCount;
        uint64_t ownerCumulativeUploadSize;
        std::vector<ActiveDataModification> activeDataModifications;
        std::vector<CompletedDataModification> completedDataModifications;
        std::vector<ConfirmedUsedSize> confirmedUsedSizes;
        std::vector<std::string> replicators;
        std::vector<std::string> offboardingReplicators;
        std::vector<Verification> verifications;
    };

	class Drive {
	public:
		DriveData data;
	};

	class MultipleDrives {
	public:
		std::vector<Drive> drives;
	};
}