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

    class DriveInfo {
	public:
		std::string drive;
		std::string LastApprovedDataModificationId;
		bool DataModificationIdIsValid;
		uint64_t InitialDownloadWork;
	};

    class Replicator {
    public:
        std::string replicatorKey;
		uint32_t version;
		uint64_t capacity;
		std::string blskey;
		std::vector<DriveInfo> drives;
    };
}