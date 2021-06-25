/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#pragma once

#include <xpxchaincpp/model/transaction_simple/transaction_container.h>
#include <xpxchaincpp/model/transaction_simple/transaction_info.h>
#include <xpxchaincpp/model/transaction_simple/transaction_status.h>

namespace xpx_chain_sdk::internal::network {
	class Context;
	class RequestParamsBuilder;
}

namespace xpx_chain_sdk {

	using internal::network::Context;
	using internal::network::RequestParamsBuilder;

	class TransactionService {
	public:
		TransactionService(
				std::shared_ptr<Config> config,
				std::shared_ptr<internal::network::Context> context,
				std::shared_ptr<RequestParamsBuilder> builder);
		~TransactionService() = default;
		std::shared_ptr<transactions_info::BasicTransaction> getTransactionInfo(const std::string& id);
		transactions_info::TransactionContainer getTransactionInfos(const std::vector<std::string>& id);
		transactions_info::TransactionStatus getTransactionStatus(const std::string& id);
		transactions_info::MultipleTransactionStatus getTransactionStatuses(const std::vector<std::string>& id);
		bool announceNewTransaction(const std::vector<uint8_t> &payload);
		bool announceAggregateBoundedTransaction(const std::vector<uint8_t> &payload);
		bool announceCosignatureTransaction(const std::vector<uint8_t> &payload);

	private:
		std::shared_ptr<Config> _config;
		std::shared_ptr<internal::network::Context> _context;
		std::shared_ptr<RequestParamsBuilder> _builder;
	};
}
