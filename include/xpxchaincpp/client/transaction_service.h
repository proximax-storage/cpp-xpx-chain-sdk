/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#pragma once

#include <xpxchaincpp/model/transaction_simple/transaction_container.h>
#include <xpxchaincpp/model/transaction_simple/transaction_info.h>
#include <xpxchaincpp/model/transaction_simple/transaction_status.h>
#include <xpxchaincpp/model/transaction_simple/transactions_page.h>
#include <xpxchaincpp/model/transaction_simple/transactions_page_options.h>

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
				const Config& config,
				std::shared_ptr<internal::network::Context> context);
		~TransactionService() = default;

        std::shared_ptr<transactions_info::BasicTransaction> getAnyTransactionInfo(const std::string& id);
		std::shared_ptr<transactions_info::BasicTransaction> getTransactionInfo(TransactionGroup group, const std::string& id);
        transactions_info::TransactionContainer getTransactionInfos(TransactionGroup group, const std::vector<std::string>& id);
		transactions_info::TransactionStatus getTransactionStatus(const std::string& id);
		transactions_info::MultipleTransactionStatus getTransactionStatuses(const std::vector<std::string>& id);
		transactions_page::TransactionsPage getTransactionsByGroup(TransactionGroup group, const TransactionsPageOptions& options = TransactionsPageOptions());
		bool announceNewTransaction(const std::vector<uint8_t> &payload);
		bool announceAggregateBoundedTransaction(const std::vector<uint8_t> &payload);
		bool announceCosignatureTransaction(const std::vector<uint8_t> &payload);

        static std::string transactionGroupToString(TransactionGroup group) ;
        static TransactionGroup transactionGroupFromString(const std::string& group) ;

	private:
		const Config& _config;
		std::shared_ptr<internal::network::Context> _context;
	};
}
