
#pragma once

#include <nemcpp/model/transaction/secret_hash_algorithm.h>
#include <nemcpp/model/transaction/transaction.h>
#include <nemcpp/model/transaction/embedded_transaction.h>

#include <memory>
#include <optional>
#include <utility>
#include <vector>

namespace nem2_sdk {
	
	/// Secret proof transaction base class.
	/// \note Unlocks funds locked with secret lock transaction for some recipient.
	template<typename TBase>
	class TSecretProofTransaction: public TBase {
	public:
		
		/// Creates secret proof transaction.
		template<typename... TArgs>
		explicit TSecretProofTransaction(SecretHashAlgorithm secretHashAlgorithm,
		                                 const Hash256& secretHash,
		                                 RawBuffer secret,
		                                 TArgs&&... args):
			TBase(TransactionType::Secret_Proof, std::forward<TArgs>(args)...),
			secretHashAlgorithm_(secretHashAlgorithm),
			secretHash_(secretHash),
			secret_(secret.begin(), secret.end()),
			secretBuffer_(secret_)
		{ }
		
		/// Returns secret hash algorithm.
		SecretHashAlgorithm secretHashAlgorithm() const;
		
		/// Returns secret hash.
		const Hash256& secretHash() const;
		
		/// Returns secret data.
		const RawBuffer& secret() const;
		
	private:
		SecretHashAlgorithm secretHashAlgorithm_;
		Hash256 secretHash_;
		std::vector<uint8_t> secret_;
		RawBuffer secretBuffer_;
	};
	
	extern template class TSecretProofTransaction<Transaction>;
	extern template class TSecretProofTransaction<EmbeddedTransaction>;
	
	using SecretProofTransaction = TSecretProofTransaction<Transaction>;
	using EmbeddedSecretProofTransaction = TSecretProofTransaction<EmbeddedTransaction>;
	
	/// Creates secret proof transaction.
	/// \note Throws \c transaction_error if \a secret size is invalid.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<SecretProofTransaction>
	CreateSecretProofTransaction(SecretHashAlgorithm secretHashAlgorithm,
	                             RawBuffer secret,
	                             std::optional<Amount> maxFee = std::nullopt,
	                             std::optional<NetworkDuration> deadline = std::nullopt,
	                             std::optional<NetworkIdentifier> networkId = std::nullopt);
	
	/// Creates embedded secret proof transaction.
	/// \note Throws \c transaction_error if  \a secret size is invalid.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedSecretProofTransaction>
	CreateEmbeddedSecretProofTransaction(SecretHashAlgorithm secretHashAlgorithm,
	                                     RawBuffer secret,
	                                     const Key& signer,
	                                     std::optional<NetworkIdentifier> networkId = std::nullopt);
}
