
#pragma once

#include <xpxchaincpp/utils/lazy.h>
#include <xpxchaincpp/utils/network_time.h>
#include <xpxchaincpp/crypto/key_pair.h>
#include <xpxchaincpp/model/transaction/basic_transaction.h>
#include <xpxchaincpp/model/transaction/transaction_info.h>

#include <memory>
#include <optional>
#include <vector>

namespace xpx_chain_sdk {
	
	class Transaction;
	
	/// Pointer to general transaction.
	using TransactionPtr = std::unique_ptr<Transaction>;
	
	/// Base class for general transactions.
	class Transaction: public BasicTransaction {
	public:
		/// Creates general transaction.
		Transaction(TransactionType type,
		            uint32_t fullVersion,
		            Amount maxFee,
		            int64_t deadline,
		            std::vector<uint8_t> binaryData,
		            const std::optional<Key>& signer = std::nullopt,
		            const std::optional<Signature>& signature = std::nullopt,
		            const std::optional<TransactionInfo>& info = std::nullopt);
		
		/// Copy constructor.
		Transaction(const Transaction& rhs);
		
		/// Move constructor.
		Transaction(Transaction&& rhs) noexcept;
		
		/// Default copy assignment operator.
		Transaction& operator=(const Transaction&) = default;
		
		/// Default move assignment operator.
		Transaction& operator=(Transaction&&) = default;
		
		/// Returns \c false.
		bool isEmbedded() const final;
		
		/// Returns \c true if transaction is signed.
		bool isSigned() const;
		
		/// Returns transaction max fee in microXEM (10^(-6) of XEM).
		/// \note Actual transaction fee is determined after it is included in block.
		Amount maxFee() const;
		
		/// Returns transaction deadline.
		NetworkTimepoint deadline() const;
		
		/// Returns transaction signature.
		/// \note Throws \c transaction_error if transaction is not signed.
		const Signature& signature() const;
		
		/// Returns transaction hash.
		/// \note Throws \c transaction_error if transaction is not signed.
		const Hash256& hash() const;
		
		/// Returns transaction binary data.
		const std::vector<uint8_t>& binary() const;
		
		/// Returns transaction info.
		const TransactionInfo& info() const;
		
	public:
		/// Returns default fee for transaction of specified size.
		static Amount GetDefaultFee(size_t size);
		
		/// Returns default transaction deadline.
		static NetworkDuration GetDefaultDeadline();
		
	private:
		using BasicTransaction::setSigner; //not needed in descendants
		friend void SignTransaction(Transaction* transaction, const KeyPair& keyPair);
		
		void initSignature(std::optional<Signature>& value);
		void initHash(std::optional<Hash256>& value);
		
	private:
		Lazy<Signature, Transaction, &Transaction::initSignature> signature_;
		Amount maxFee_;
		NetworkTimepoint deadline_;
		
		Lazy<Hash256, Transaction, &Transaction::initHash> hash_;
		std::vector<uint8_t> binary_;
		
		TransactionInfo info_;
	};
}
