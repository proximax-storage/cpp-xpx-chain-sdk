/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/types.h>

#include <string>
#include <string_view>

namespace xpx_chain_sdk {

	struct ProofOfExecution {
		uint64_t startBatchId = 0;
		std::array<uint8_t, 32> T;
		std::array<uint8_t, 32> R;
	};

	struct ExecutorInfo {
		uint64_t nextBatchToApprove = 0;
		ProofOfExecution poEx;
	};

}