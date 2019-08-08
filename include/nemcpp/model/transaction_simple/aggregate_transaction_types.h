
#pragma once

#include <nemcpp/model/account/public_account.h>
//#include <nemcpp/model/transaction/embedded_transaction.h>

#include <unordered_map>

namespace xpx_sdk::simple_transactions {
	
	/// Transactions, embedded to aggregate transaction.
//	using EmbeddedTransactions = std::vector<std::shared_ptr<EmbeddedTransaction>>;
	
	/// Aggregate transaction cosignatures.
	using Cosignatures = std::unordered_map<PublicAccount, Signature>;
}