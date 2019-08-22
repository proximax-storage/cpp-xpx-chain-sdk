#pragma once

/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/


#include <nemcpp/model/transaction_simple/transaction_container.h>
#include <nemcpp/model/transaction_simple/transaction_info.h>
#include <nemcpp/model/transaction_simple/transaction_status.h>

namespace xpx_sdk::internal::network {
	class Context;
	class RequestParamsBuilder;
}

namespace xpx_sdk {

	using internal::network::Context;
	using internal::network::RequestParamsBuilder;

	class TransactionService {
	public:
		TransactionService(
				std::shared_ptr<Config> config,
				std::shared_ptr<internal::network::Context> context,
				std::shared_ptr<RequestParamsBuilder> builder);
		~TransactionService() = default;
		std::shared_ptr<simple_transactions::BasicTransaction> getTransactionInfo(const std::string& id);
		simple_transactions::TransactionContainer getTransactionInfos(const std::vector<std::string>& id );
		simple_transactions::TransactionStatus getTransactionStatus(const std::string& id);
		simple_transactions::MultipleTransactionStatus getTransactionStatuses(const std::vector<std::string>& id);
		bool announceNewTransaction(const std::vector<uint8_t> &payload);
		bool announceAggregateBoundedTransaction(const std::vector<uint8_t> &payload);
		bool announceCosignatureTransaction(const std::vector<uint8_t> &payload);

	private:
		std::shared_ptr<Config> _config;
		std::shared_ptr<internal::network::Context> _context;
		std::shared_ptr<RequestParamsBuilder> _builder;
	};
}
