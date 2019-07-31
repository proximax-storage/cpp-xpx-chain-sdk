
#pragma once

#include <nemcpp/model/transaction/aggregate_transaction_types.h>
#include <nemcpp/model/transaction/transaction.h>

#include <memory>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace xpx_sdk {
	
	/// Aggregate transaction.
	/// \note If aggregate transaction will be announced to network with all cosignatures, it is called complete.
	///       Otherwise aggregate transaction is called bonded and should be signed by other cosigners.
	/// \note Prior to bonded aggregate transaction announcement lock funds transaction should be sent (in order
	///       to prevent transaction spamming).
	class AggregateTransaction: public Transaction {
	public:
		///Creates aggregate transaction.
		template<typename... TArgs>
		AggregateTransaction(bool isComplete,
		                     const EmbeddedTransactions& embeddedTransactions,
		                     const std::unordered_map<Key, Signature>& cosignatures,
		                     size_t payloadSize,
		                     TArgs&&... args):
			Transaction(isComplete ? TransactionType::Aggregate_Complete : TransactionType::Aggregate_Bonded,
			            std::forward<TArgs>(args)...),
			embeddedTransactions_(embeddedTransactions),
			payloadSize_(payloadSize)
		{
			for (const auto& tx: embeddedTransactions_) {
				cosigners_.insert(tx->signer());
			}
			
			for (const auto& [key, signature]: cosignatures) {
				cosignatures_[PublicAccount{ key, networkId() }] = signature;
			}
		}
		
		/// Returns \c true if aggregate transaction is complete.
		bool isComplete() const;
		
		/// Returns embedded transactions.
		const EmbeddedTransactions& embeddedTransactions() const;
		
		/// Returns signing accounts for all embedded transactions.
		const std::unordered_set<PublicAccount>& cosigners() const;
		
		/// Returns all cosignatures including initial one (if any).
		const Cosignatures& cosignatures() const;
		
		/// Returns aggregate transaction payload size (size of all embedded transactions without cosignatures).
		size_t payloadSize() const;
		
	private:
		EmbeddedTransactions embeddedTransactions_;
		std::unordered_set<PublicAccount> cosigners_;
		mutable Cosignatures cosignatures_;
		size_t payloadSize_;
	};
	
	/// Creates complete aggregate transaction.
	/// \note Throws \c transaction_error if \a embeddedTransactions has invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<AggregateTransaction>
	CreateCompleteAggregateTransaction(const EmbeddedTransactions& embeddedTransactions,
	                                   const std::unordered_map<Key, Signature>& cosignatures,
	                                   std::optional<Amount> maxFee = std::nullopt,
	                                   std::optional<NetworkDuration> deadline = std::nullopt,
	                                   std::optional<NetworkIdentifier> networkId = std::nullopt);
	
	/// Creates bonded aggregate transaction.
	/// \note Throws \c transaction_error if \a embeddedTransactions has invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<AggregateTransaction>
	CreateBondedAggregateTransaction(const EmbeddedTransactions& embeddedTransactions,
	                                 const std::unordered_map<Key, Signature>& cosignatures,
	                                 std::optional<Amount> maxFee = std::nullopt,
	                                 std::optional<NetworkDuration> deadline = std::nullopt,
	                                 std::optional<NetworkIdentifier> networkId = std::nullopt);
}
