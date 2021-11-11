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

    struct DriveInfo {
		/// Identifier of the most recent data modification of the drive approved by the replicator.
		std::string LastApprovedDataModificationId;

		/// Indicates if \p LastApprovedDataModificationId is an identifier of an existing data modification.
		/// Can be \b false only if the drive had no approved data modifications when the replicator joined it.
		/// Set to \b true after replicator’s first data modification approval.
		bool DataModificationIdIsValid;

		/// Used drive size at the time of the replicator’s onboarding excluding metafiles size.
		/// Set to \p 0 after replicator’s first data modification approval.
		uint64_t InitialDownloadWork;

        uint16_t index;
	};

    class Replicator {
    public:
        std::string replicatorKey;
		int32_t version;
		uint64_t capacity;
		BLSPublicKey blskey;
		std::vector<DriveInfo> drives;
    };
}