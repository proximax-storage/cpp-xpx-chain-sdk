/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <infrastructure/network/http.h>
#include <infrastructure/network/context.h>
#include <nemcpp/config.h>
#include <nemcpp/client/blockchain_service.h>

namespace nem2_sdk {

	class BlockchainService : public IBlockchain {
	public:
		BlockchainService(
				std::shared_ptr<Config> config,
				std::shared_ptr<internal::network::Context> context,
				internal::network::RequestParamsBuilder builder
		);

		uint64_t getBlockchainHeight() override;
        ScoreInfo getCurrentScore() override;
        StorageInfo getStorageInfo() override;
		Block getBlockByHeight(uint64_t height) override;
		std::vector<Block> getBlocksByHeightWithLimit(uint64_t height, uint64_t limit) override;


	private:
		std::shared_ptr<Config> _config;
		std::shared_ptr<internal::network::Context> _context;
		internal::network::RequestParamsBuilder _builder;
	};

}
