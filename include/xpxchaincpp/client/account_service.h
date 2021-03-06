/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/model/account/account_info.h>
#include <xpxchaincpp/model/account/multiple_account_info.h>
#include <xpxchaincpp/model/account/account_properties.h>
#include <xpxchaincpp/model/account/multisig_info.h>
#include <xpxchaincpp/model/account/multisig_graph.h>
#include <xpxchaincpp/model/transaction_simple/transaction_container.h>

namespace xpx_chain_sdk::internal::network {
	class Context;
	class RequestParamsBuilder;
}

namespace xpx_chain_sdk {

	using internal::network::Context;
	using internal::network::RequestParamsBuilder;
	using transactions_info::TransactionContainer;

    class AccountService {
    public:
        AccountService(
                std::shared_ptr<Config> config,
                std::shared_ptr<internal::network::Context> context,
				std::shared_ptr<RequestParamsBuilder> builder);
        ~AccountService() = default;

        AccountInfo getAccountInfo(const std::string& id);
        MultipleAccountInfo getAccountsInfo(const std::vector<std::string>& ids);
        AccountProperties getAccountProperties(const std::string& id );
        MultipleAccountProperty getAccountsProperties(const std::vector<std::string>& ids);
        MultisigInfo getMultisigInfo(const std::string& id);
        MultisigGraph getMultisigAccountGraphInfo(const std::string& id);
		TransactionContainer getAccountTransactions(
				const std::string& publicKey,
				int pageSize =10,
				std::string id = "underfined",
				std::string ordering = "-id");
		TransactionContainer getAccountIncomingTransactions(
				const std::string& publicKey,
				int pageSize =10,
				std::string id = "underfined",
				std::string ordering = "-id");
		TransactionContainer getAccountOutgoingTransactions(
				const std::string& publicKey,
				int pageSize =10,
				std::string id = "underfined",
				std::string ordering = "-id");
		TransactionContainer getAccountUnconfirmedTransactions(
				const std::string& publicKey,
				int pageSize =10,
				std::string id = "underfined",
				std::string ordering = "-id");
		TransactionContainer getAccountAggregateBoundedTransactions(
				const std::string& publicKey,
				int pageSize =10,
				std::string id = "underfined",
				std::string ordering = "-id");
    private:
        std::shared_ptr<Config> _config;
        std::shared_ptr<internal::network::Context> _context;
		std::shared_ptr<RequestParamsBuilder> _builder;
    };
}


