/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <nemcpp/model/account/account_info.h>
#include <string>
#include <nemcpp/model/account/multisig_info.h>
#include <nemcpp/model/account/account_info.h>
#include <nemcpp/model/account/multiple_account_info.h>
#include <nemcpp/model/account/multisig_graph.h>
#include <nemcpp/model/account/account_properties.h>
#include <nemcpp/model/transaction_simple/transaction_container.h>

namespace xpx_sdk {
    class IAccountService {
    public:
        virtual AccountInfo getAccountInfo(const std::string& id) = 0;
        virtual MultipleAccountInfo getAccountsInfo(const std::vector<std::string>& ids) = 0;
        virtual AccountProperty getAccountProperties(const std::string& id ) = 0;
        virtual MultipleAccountProperty getAccountsProperties(const std::vector<std::string>& ids) = 0;
        virtual MultisigInfo getMultisigInfo(const std::string& id) = 0;
        virtual MultisigGraph getMultisigAccountGraphInfo(const std::string& id) = 0;
        virtual simple_transactions::TransactionContainer getAccountTransactions(
        		const std::string& publicKey,
        		int pageSize =10,
        		std::string id = "underfined",
        		std::string ordering = "-id") = 0;
		virtual simple_transactions::TransactionContainer getAccountIncomingTransactions(
				const std::string& publicKey,
				int pageSize =10,
				std::string id = "underfined",
				std::string ordering = "-id") = 0;
		virtual simple_transactions::TransactionContainer getAccountOutgoingTransactions(
				const std::string& publicKey,
				int pageSize =10,
				std::string id = "underfined",
				std::string ordering = "-id") = 0;
		virtual simple_transactions::TransactionContainer getAccountUnconfirmedTransactions(
				const std::string& publicKey,
				int pageSize =10,
				std::string id = "underfined",
				std::string ordering = "-id") = 0;
		virtual simple_transactions::TransactionContainer getAccountAggregateBoundedTransactions(
				const std::string& publicKey,
				int pageSize =10,
				std::string id = "underfined",
				std::string ordering = "-id") = 0;
        virtual ~IAccountService() = default;
    };
}