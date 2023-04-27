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

	struct CompletedCall {
		std::string callId;
		std::string caller;
		int16_t status;
        uint64_t executionWork;
        uint64_t downloadWork;
	};

	struct Batch {
		bool success;
		std::array<uint8_t, 32> poExVerificationInformation;
		std::vector<CompletedCall> completedCalls;
	};
}