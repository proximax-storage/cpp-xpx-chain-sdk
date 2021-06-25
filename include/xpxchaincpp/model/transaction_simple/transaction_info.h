/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <string>
#include <vector>

namespace xpx_chain_sdk::transactions_info {

	class TransactionInfo {
	public:
        uint64_t height;
        uint32_t index;
        std::string id;
        std::string hash;
        std::string merkleComponentHash;
        std::string aggregateHash;
        std::string uniqueAggregateHash;
        std::string aggregateId;
	};

	class MultipleTransactionInfo {
	public:
		std::vector<TransactionInfo> infos;
	};
}