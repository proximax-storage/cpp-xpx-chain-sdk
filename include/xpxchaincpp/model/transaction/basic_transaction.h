/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/types.h>
#include <xpxchaincpp/utils/lazy.h>
#include <xpxchaincpp/model/account/public_account.h>
#include <xpxchaincpp/model/transaction/transaction_type.h>

#include <memory>
#include <optional>

namespace xpx_chain_sdk {
	
	class BasicTransaction;
	
	/// Pointer to basic transaction.
	using BasicTransactionPtr = std::unique_ptr<BasicTransaction>;
	
	/// Base class for embedded and general transactions.
	class BasicTransaction {
	public:
		/// Base constructor for transactions.
		BasicTransaction(TransactionType type, uint32_t fullVersion, const std::optional<Key>& signer);
		
		/// Copy constructor.
		BasicTransaction(const BasicTransaction& rhs);
		
		/// Move constructor.
		BasicTransaction(BasicTransaction&& rhs) noexcept;
		
		/// Default copy assignment operator.
		BasicTransaction& operator=(const BasicTransaction&) = default;
		
		/// Default move assignment operator.
		BasicTransaction& operator=(BasicTransaction&&) = default;
		
		/// Allows correct descendants destruction.
		virtual ~BasicTransaction() = 0;
		
		/// Returns \c true of transaction is embedded.
		virtual bool isEmbedded() const = 0;
		
		/// Returns transaction type.
		TransactionType type() const;
		
		/// Returns transaction version.
		uint8_t version() const;
		
		/// Returns network for which transaction was created.
		NetworkIdentifier networkId() const;
		
		/// Returns transaction signer public account.
		/// \note Throws \c transaction_error if transaction is not signed.
		const PublicAccount& signer() const;
		
	protected:
		void setSigner(const Key& publicKey);
		
	private:
		void initSigner(std::optional<PublicAccount>& value);
		
	private:
		Lazy<PublicAccount, BasicTransaction, &BasicTransaction::initSigner> signer_;
		uint32_t version_;
		NetworkIdentifier networkId_;
		TransactionType type_;
	};
}
