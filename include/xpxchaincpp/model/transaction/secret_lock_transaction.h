/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/model/mosaic/mosaic.h>
#include <xpxchaincpp/model/transaction/secret_hash_algorithm.h>
#include <xpxchaincpp/model/transaction/transaction.h>
#include <xpxchaincpp/model/transaction/embedded_transaction.h>

#include <memory>
#include <optional>
#include <utility>

namespace xpx_chain_sdk {
	
	/// Secret lock transaction base class.
	/// \note Locks funds using secret random set of bytes. Funds are unlocked with proof transaction (in which
	///       case recipient specified in lock transaction will receive them) or automatically after lock duration
	///       is reached (in which case funds will be returned to sender).
	template<typename TBase>
	class TSecretLockTransaction: public TBase {
	public:
		/// Creates secret lock transaction.
		template<typename... TArgs>
		explicit TSecretLockTransaction(SecretHashAlgorithm secretHashAlgorithm,
		                                const Hash256& secretHash,
		                                const Mosaic& lockedMosaic,
		                                BlockDuration lockDuration,
		                                const Address& lockedMosaicRecipient,
		                                TArgs&&... args):
			TBase(TransactionType::Secret_Lock, std::forward<TArgs>(args)...),
			lockedMosaic_(lockedMosaic),
			lockDuration_(lockDuration),
			secretHashAlgorithm_(secretHashAlgorithm),
			secretHash_(secretHash),
			lockedMosaicRecipient_(lockedMosaicRecipient)
		{ }
		
		/// Returns secret hash.
		const Hash256& secretHash() const;
		
		/// Returns secret hash algorithm.
		SecretHashAlgorithm secretHashAlgorithm() const;
		
		/// Returns locked mosaic.
		const Mosaic& lockedMosaic() const;
		
		/// Returns lock duration.
		BlockDuration lockDuration() const;
		
		/// Returns locked mosaic recipient.
		const Address& lockedMosaicRecipient() const;
		
	private:
		Mosaic lockedMosaic_;
		BlockDuration lockDuration_;
		SecretHashAlgorithm secretHashAlgorithm_;
		Hash256 secretHash_;
		Address lockedMosaicRecipient_;
	};
	
	extern template class TSecretLockTransaction<Transaction>;
	extern template class TSecretLockTransaction<EmbeddedTransaction>;
	
	using SecretLockTransaction = TSecretLockTransaction<Transaction>;
	using EmbeddedSecretLockTransaction = TSecretLockTransaction<EmbeddedTransaction>;
	
//	/// Creates secret lock transaction.
//	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
//	std::unique_ptr<SecretLockTransaction>
//	CreateSecretLockTransaction(SecretHashAlgorithm secretHashAlgorithm,
//	                            const Hash256& secretHash,
//	                            const Mosaic& lockedMosaic,
//	                            BlockDuration lockDuration,
//	                            const Address& lockedMosaicRecipient,
//	                            std::optional<Amount> maxFee = std::nullopt,
//	                            std::optional<NetworkDuration> deadline = std::nullopt,
//	                            std::optional<NetworkIdentifier> networkId = std::nullopt);
//
//	/// Creates secret lock transaction from secret.
//	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
//	std::unique_ptr<SecretLockTransaction>
//	CreateSecretLockTransactionFromSecret(SecretHashAlgorithm secretHashAlgorithm,
//	                                      RawBuffer secret,
//	                                      const Mosaic& lockedMosaic,
//	                                      BlockDuration lockDuration,
//	                                      const Address& lockedMosaicRecipient,
//	                                      std::optional<Amount> maxFee = std::nullopt,
//	                                      std::optional<NetworkDuration> deadline = std::nullopt,
//	                                      std::optional<NetworkIdentifier> networkId = std::nullopt);
//
//	/// Creates embedded secret lock transaction.
//	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
//	std::unique_ptr<EmbeddedSecretLockTransaction>
//	CreateEmbeddedSecretLockTransaction(SecretHashAlgorithm secretHashAlgorithm,
//	                                    const Hash256& secretHash,
//	                                    const Mosaic& lockedMosaic,
//	                                    BlockDuration lockDuration,
//	                                    const Address& lockedMosaicRecipient,
//	                                    const Key& signer,
//	                                    std::optional<NetworkIdentifier> networkId = std::nullopt);
//
//	/// Creates embedded secret lock transaction from secret.
//	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
//	std::unique_ptr<EmbeddedSecretLockTransaction>
//	CreateEmbeddedSecretLockTransactionFromSecret(SecretHashAlgorithm secretHashAlgorithm,
//	                                              RawBuffer secret,
//	                                              const Mosaic& lockedMosaic,
//	                                              BlockDuration lockDuration,
//	                                              const Address& lockedMosaicRecipient,
//	                                              const Key& signer,
//	                                              std::optional<NetworkIdentifier> networkId = std::nullopt);
}
