#pragma once

#include <nemcpp/config.h>

#include <vector>

namespace nem2_sdk {
	class Blockchain {
	public:
		Blockchain(std::shared_ptr<Config> config);

//		BlockInfoDto getBlockByHeight(uint64_t height);
//		std::vector<TransactionDto> getBlockTransactions(uint64_t height);
//		std::vector<BlockInfoDto> getBlocksByHeightWithLimit(uint64_t height, uint64_t limit);
		uint64_t getBlockchainHeight();

	private:
		std::shared_ptr<Config> _config;
	};
}
