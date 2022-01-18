/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <stdint.h>

namespace xpx_chain_sdk {

    class DriveData {
    public:
		std::string multisig;
		std::string owner;
		std::string rootHash;
        uint64_t size;
        uint64_t usedSize;
		std::vector<std::string> replicators;
		std::vector<std::string> offboardingReplicators;
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