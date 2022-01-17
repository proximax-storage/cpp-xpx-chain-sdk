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
		/// Creates onboarding transaction.
		template<typename... TArgs>
		explicit TReplicatorOnboardingTransaction(
				const Amount& capacity,
		        TArgs&&... args):
			TBase(TransactionType::Replicator_Onboarding, std::forward<TArgs>(args)...),
			capacity_(capacity)
		{ }

		/// Returns capacity that the replicator is willing to contribute (Megabytes).
		const Amount& capacity() const;

	private:
		Amount capacity_;
	};

	extern template class TReplicatorOnboardingTransaction<Transaction>;
	extern template class TReplicatorOnboardingTransaction<EmbeddedTransaction>;

	using ReplicatorOnboardingTransaction = TReplicatorOnboardingTransaction<Transaction>;
	using EmbeddedReplicatorOnboardingTransaction = TReplicatorOnboardingTransaction<EmbeddedTransaction>;

	/// Creates replicator onboarding transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<ReplicatorOnboardingTransaction>
    CreateReplicatorOnboardingTransaction(const Amount& capacity,
                                          std::optional<Amount> maxFee = std::nullopt,
                                          std::optional<NetworkDuration> deadline = std::nullopt,
                                          std::optional<NetworkIdentifier> networkId = std::nullopt);


	/// Creates replicator onboarding transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedReplicatorOnboardingTransaction>
    CreateEmbeddedReplicatorOnboardingTransaction(const Amount& capacity,
                                                  const Key& signer,
                                                  std::optional<NetworkIdentifier> networkId = std::nullopt);
}
