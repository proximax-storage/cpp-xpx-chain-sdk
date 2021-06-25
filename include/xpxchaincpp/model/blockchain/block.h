/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <string>

namespace xpx_chain_sdk {

	class BlockMeta {
	public:
		std::string hash;
		std::string generationHash;
		uint64_t totalFee;
		uint64_t numTransactions;
	};

	class BlockData {
	public:
		std::string signature;
		uint64_t timestamp;
		uint64_t difficulty;
		uint32_t feeMultiplier;
		std::string previousBlockHash;
		std::string blockTransactionsHash;
		std::string blockReceiptsHash;
		std::string stateHash;
		std::string beneficiaryPublicKey;
	};

	class Block {
	public:
		BlockMeta meta;
		BlockData data;
	};
}
