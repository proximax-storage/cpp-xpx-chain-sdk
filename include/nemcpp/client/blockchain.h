#pragma once

#include <nemcpp/model/blockchain/block.h>

namespace nem2_sdk {
	class IBlockchain {
	public:
		virtual uint64_t getBlockchainHeight() = 0;
		virtual Block getBlockByHeight(uint64_t height) = 0;
		virtual std::vector<Block> getBlocksByHeightWithLimit(
				uint64_t height,
				uint64_t limit
		) = 0;
		virtual ~IBlockchain() = default;
	};
}
