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

	struct ServicePayment {
        uint64_t mosaicId;
		uint64_t amount;
	};

	struct ContractCall {
        std::string callId;
		std::string caller;
		std::string fileName;
		std::string functionName;
		std::string actualArguments;
		uint64_t executionCallPayment;
		uint64_t downloadCallPayment;
		std::vector<ServicePayment> servicePayments;
        uint64_t blockHeight;
	};
}