/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <nemcpp/config.h>
#include <nemcpp/client/blockchain_service.h>
#include <nemcpp/model/blockchain/storage_info.h>
#include <nemcpp/model/blockchain/block.h>
#include <nemcpp/model/transaction_simple/transaction_container.h>
#include <nemcpp/model/blockchain/multiple_block.h>
#include <nemcpp/model/blockchain/score.h>


namespace xpx_sdk::internal::network {
	class Context;
	class RequestParamsBuilder;
}

namespace xpx_sdk {

	using internal::network::Context;
	using internal::network::RequestParamsBuilder;
	using simple_transactions::TransactionContainer;

	class BlockchainService {
	public:
		BlockchainService(
				std::shared_ptr<Config> config,
				std::shared_ptr<internal::network::Context> context,
				std::shared_ptr<RequestParamsBuilder>  builder
		);
		~BlockchainService() = default;
		uint64_t getBlockchainHeight();
        ScoreInfo getCurrentScore();
        StorageInfo getStorageInfo();
		Block getBlockByHeight(uint64_t height);
		TransactionContainer getBlockTransactions(uint64_t height);
        MultipleBlock getBlocksByHeightWithLimit(uint64_t height, uint64_t limit);


	private:
		std::shared_ptr<Config> _config;
		std::shared_ptr<Context> _context;
		std::shared_ptr<RequestParamsBuilder> _builder;
	};

}
