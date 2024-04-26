/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/utils/buffer.h>
#include <xpxchaincpp/model/transaction/transaction.h>
#include <xpxchaincpp/model/transaction/embedded_transaction.h>

#include <memory>
#include <optional>
#include <string>
#include <utility>

namespace xpx_chain_sdk {

	/// Replicator onboarding transaction base class.
	template<typename TBase>
	class TReplicatorOnboardingTransaction: public TBase {
	public:
		/// Creates replicator onboarding transaction.
		template<typename... TArgs>
		explicit TReplicatorOnboardingTransaction(
				const Amount& capacity,
				const Key& nodeBootKey,
				const Hash256& message,
				const Signature& messageSignature,
		        TArgs&&... args):
			TBase(TransactionType::Data_Modification, std::forward<TArgs>(args)...),
			capacity_(capacity),
			nodeBootKey_(nodeBootKey),
			message_(message),
			messageSignature_(messageSignature)
		{ }

		/// Returns capacity that the replicator is willing to contribute.
		const Amount& capacity() const;

		/// The boot public key of the node where the replicator will be running.
		const Key& nodeBootKey() const;

		/// A random message to be signed by the node's boot private key.
		const Hash256& message() const;

		/// The message signature.
		const Signature& messageSignature() const;

	private:
		Amount capacity_;
		Key nodeBootKey_;
		Hash256 message_;
		Signature messageSignature_;
	};

	extern template class TReplicatorOnboardingTransaction<Transaction>;
	extern template class TReplicatorOnboardingTransaction<EmbeddedTransaction>;

	using ReplicatorOnboardingTransaction = TReplicatorOnboardingTransaction<Transaction>;
	using EmbeddedReplicatorOnboardingTransaction = TReplicatorOnboardingTransaction<EmbeddedTransaction>;

	/// Creates replicator onboarding transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<ReplicatorOnboardingTransaction>
	CreateReplicatorOnboardingTransaction(
		const Amount& capacity,
		const Key& nodeBootKey,
		const Hash256& message,
		const Signature& messageSignature,
		std::optional<Amount> maxFee = std::nullopt,
		std::optional<NetworkDuration> deadline = std::nullopt,
		std::optional<NetworkIdentifier> networkId = std::nullopt);


	/// Creates replicator onboarding transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedReplicatorOnboardingTransaction>
	CreateEmbeddedReplicatorOnboardingTransaction(
		const Amount& capacity,
		const Key& nodeBootKey,
		const Hash256& message,
		const Signature& messageSignature,
		const Key& signer,
		std::optional<NetworkIdentifier> networkId = std::nullopt);
}
