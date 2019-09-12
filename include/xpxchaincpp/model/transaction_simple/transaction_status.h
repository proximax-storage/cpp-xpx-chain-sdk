/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <string>
#include <vector>

namespace xpx_chain_sdk::transactions_info {
	class TransactionStatus {
	public:
		std::string group, status, hash;
		uint64_t deadline, height;
	};

	class MultipleTransactionStatus {
	public:
		std::vector<TransactionStatus> statuses;
	};
}