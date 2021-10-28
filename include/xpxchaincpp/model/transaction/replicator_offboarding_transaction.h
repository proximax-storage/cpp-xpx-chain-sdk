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

	/// Replicator offboarding transaction base class.
	template<typename TBase>
	class TReplicatorOffboardingTransaction: public TBase {
	public:
		/// Creates download transaction.
		template<typename... TArgs>
		explicit TReplicatorOnboardingTransaction(
		        TArgs&&... args):
			TBase(TransactionType::Replicator_Offboarding, std::forward<TArgs>(args)...),
		{ }
	};
	extern template class TReplicatorOffboardingTransaction<Transaction>;
	extern template class TReplicatorOffboardingTransaction<EmbeddedTransaction>;

	using ReplicatorOffboardingTransaction = TReplicatorOffboardingTransaction<Transaction>;
	using EmbeddedReplicatorOffboardingTransaction = TReplicatorOffboardingTransaction<EmbeddedTransaction>;

	/// Creates replicator offboarding transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<ReplicatorOffboardingTransaction>
	CreateReplicatorOffboardingTransaction(std::optional<Amount> maxFee = std::nullopt,
	                          std::optional<NetworkDuration> deadline = std::nullopt,
	                          std::optional<NetworkIdentifier> networkId = std::nullopt);


	/// Creates replicator offboarding transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedReplicatorOffboardingTransaction>
	CreateEmbeddedReplicatorOnboardingTransaction(const Key& signer,
	                                  std::optional<NetworkIdentifier> networkId = std::nullopt);
}
