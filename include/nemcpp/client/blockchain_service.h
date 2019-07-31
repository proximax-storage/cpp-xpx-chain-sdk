/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <nemcpp/model/blockchain/block.h>
#include <nemcpp/model/blockchain/score.h>
#include <nemcpp/model/blockchain/storage_info.h>
#include <nemcpp/model/blockchain/multiple_block.h>

namespace xpx_sdk {
	class IBlockchainService {
	public:
		virtual uint64_t getBlockchainHeight() = 0;
		virtual ScoreInfo getCurrentScore() = 0;
		virtual StorageInfo getStorageInfo() = 0;
		virtual Block getBlockByHeight(uint64_t height) = 0;
		virtual MultipleBlock getBlocksByHeightWithLimit(
				uint64_t height,
				uint64_t limit
		) = 0;
		virtual ~IBlockchainService() = default;
	};
}

