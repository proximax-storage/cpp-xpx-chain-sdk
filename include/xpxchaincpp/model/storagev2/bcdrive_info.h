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
		uint64_t expectedUploadSize;
		uint64_t actualUploadSize;
		std::string folderName;
		bool readyForApprove;
	};

    class CompletedDataModification {
	public:
		std::vector<ActiveDataModification> activeDataModification;
		DataModificationState State;
	};

	class ConfirmedUsedSize {
	public:
		std::string replicator;
		uint64_t storageSize;
	};

	enum class VerificationState : uint8_t {
		PendingVerification,
		CanceledVerification,
		FinishedVerification
	};

	class VerificationOpinion {
	public:
		std::string prover;
		uint8_t result;
	};

	class Verification {
	public:
		std::string verificationTrigger;
		VerificationState verificationState;
		std::vector<VerificationOpinion> verificationOpinion;
	};

	// vector
    class ActiveDataModifications {
	public:
		std::vector<ActiveDataModification> nActiveDataModification;
	};

	class CompletedDataModifications {
	public:
		std::vector<CompletedDataModification> nCompletedDataModification;
	};

	class ConfirmedUsedSizes {
	public:
		std::vector<ConfirmedUsedSize> nConfirmedUsedSize;
	};

	class Verifications {
	public:
		std::vector<Verification> nVerification;
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
		ConfirmedUsedSizes confirmUsedSizes;
		std::vector<std::string> replicators;
		Verifications verifications;
    };
}