/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/model/transaction/basic_transaction.h>

#include <memory>

namespace xpx_chain_sdk {
	
	class EmbeddedTransaction;
	
	/// Pointer to embedded transaction.
	using EmbeddedTransactionPtr = std::unique_ptr<EmbeddedTransaction>;
	
	/// Base class for embedded transactions.
	class EmbeddedTransaction: public BasicTransaction {
	public:
		/// Creates embedded transaction.
		EmbeddedTransaction(TransactionType type, uint32_t fullVersion, const Key& signer);
		
		/// Returns \c true.
		bool isEmbedded() const final;
		
	private:
		using BasicTransaction::setSigner; //not needed in descendants
	};
}
