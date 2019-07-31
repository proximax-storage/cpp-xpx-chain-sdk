#pragma once

#include <string>
#include <infrastructure/json/dto/block_dto.h>
#include <infrastructure/utils/variadic_struct.h>

namespace xpx_sdk {

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
		BlockData data;
	};
}
