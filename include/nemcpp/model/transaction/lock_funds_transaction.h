
#pragma once

#include <nemcpp/model/mosaic/mosaic.h>
#include <nemcpp/model/transaction/transaction.h>
#include <nemcpp/model/transaction/embedded_transaction.h>

#include <memory>
#include <optional>
#include <utility>

namespace xpx_sdk::difficult_transactions {
	
	/// Lock funds transaction base class.
	/// \note Currently this transaction is used only for locking funds before announcing aggregate
	///       bonded transaction in order to prevent transaction spamming. In that case lock hash
	///       should be set to to corresponding aggregate bonded transaction hash.
	template<typename TBase>
	class TLockFundsTransaction: public TBase {
	public:
		/// Creates lock funds transaction.
		template<typename... TArgs>
		explicit TLockFundsTransaction(const Mosaic& lockedMosaic,
		                               BlockDuration lockDuration,
		                               const Hash256& lockHash,
		                               TArgs&&... args):
			TBase(TransactionType::Lock_Funds, std::forward<TArgs>(args)...),
			lockedMosaic_(lockedMosaic),
			lockDuration_(lockDuration),
			lockHash_(lockHash)
		{ }
		
		/// Returns locked mosaic.
		const Mosaic& lockedMosaic() const;
		
		/// Returns lock duration.
		/// \note If conditions, required to unlock funds, are not met until specified block height
		///       then locked funds can be harvested by any node.
		BlockDuration lockDuration() const;
		
		/// Returns lock hash.
		const Hash256& lockHash() const;
		
	private:
		Mosaic lockedMosaic_;
		BlockDuration lockDuration_;
		Hash256 lockHash_;
	};
	
	extern template class TLockFundsTransaction<Transaction>;
	extern template class TLockFundsTransaction<EmbeddedTransaction>;
	
	using LockFundsTransaction = TLockFundsTransaction<Transaction>;
	using EmbeddedLockFundsTransaction = TLockFundsTransaction<EmbeddedTransaction>;
	
	/// Creates lock funds transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<LockFundsTransaction>
	CreateLockFundsTransaction(const Mosaic& lockedMosaic,
	                           BlockDuration lockDuration,
	                           const Hash256& lockHash,
	                           std::optional<Amount> maxFee = std::nullopt,
	                           std::optional<NetworkDuration> deadline = std::nullopt,
	                           std::optional<NetworkIdentifier> networkId = std::nullopt);
	
	/// Creates embedded lock funds transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedLockFundsTransaction>
	CreateEmbeddedLockFundsTransaction(const Mosaic& lockedMosaic,
	                                   BlockDuration lockDuration,
	                                   const Hash256& lockHash,
	                                   const Key& signer,
	                                   std::optional<NetworkIdentifier> networkId = std::nullopt);
}
