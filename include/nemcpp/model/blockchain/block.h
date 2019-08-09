#pragma once

#include <string>

namespace nem2_sdk {
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
		uint64_t feeMultiplier;
		std::string previousBlockHash;
		std::string blockTransactionsHash;
		std::string blockReceiptsHash;
		std::string stateHash;
		std::string beneficiaryPublicKey;
	};

	class Block {
	public:
		BlockMeta meta;
		BlockData block;
	};
}
