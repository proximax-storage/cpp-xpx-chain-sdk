#pragma once

#include <infrastructure/network/http.h>
#include <infrastructure/network/context.h>
#include <nemcpp/config.h>
#include <nemcpp/model/blockchain/block.h>
#include <nemcpp/client/blockchain.h>

namespace nem2_sdk {

	class Blockchain : public IBlockchain {
	public:
		Blockchain(
				std::shared_ptr<Config> config,
				std::shared_ptr<internal::network::Context> context,
				internal::network::RequestParamsBuilder builder
		);

		uint64_t getBlockchainHeight() override;
		Block getBlockByHeight(uint64_t height) override;

	private:
		std::shared_ptr<Config> _config;
		std::shared_ptr<internal::network::Context> _context;
		internal::network::RequestParamsBuilder _builder;
	};

}
