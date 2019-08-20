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
	using simple_transactions::TransactionContainer;

	class TransactionService {
	public:
		TransactionService(
				std::shared_ptr<Config> config,
				std::shared_ptr<internal::network::Context> context,
				std::shared_ptr<RequestParamsBuilder> builder);
		~TransactionService() = default;
		TransactionInfo getTransactionInfo(const std::string& id);
		MultipleTransactionInfo getTransactionInfos(const std::vector<std::string>& id );
		TransactionStatus getTransactionStatus(const std::string& id);
		MultipleTransactionStatus getTransactionStatuses(const std::vector<std::string>& id);
		bool announceNewTransaction(const std::string &payload);
		bool announceAggregateBoundedTransaction(const std::string &payload);
		bool announceCosignatureTransaction(const std::string &payload);

	private:
		std::shared_ptr<Config> _config;
		std::shared_ptr<internal::network::Context> _context;
		std::shared_ptr<RequestParamsBuilder> _builder;
	};
}
