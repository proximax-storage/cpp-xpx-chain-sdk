/**
*** Copyright 2024 ProximaX Limited. All rights reserved.
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

	/// Replicators cleanup transaction base class.
	template<typename TBase>
	class TReplicatorsCleanupTransaction: public TBase {
	public:
		/// Creates replicators cleanup transaction.
		template<typename... TArgs>
		explicit TReplicatorsCleanupTransaction(
				std::vector<Key> replicatorKeys,
		        TArgs&&... args):
			TBase(TransactionType::Data_Modification, std::forward<TArgs>(args)...),
			replicatorKeys_(std::move(replicatorKeys))
		{ }

		/// Returns the public keys of the replicator to remove.
		const std::vector<Key>& replicatorKeys() const;

	private:
		std::vector<Key> replicatorKeys_;
	};

	extern template class TReplicatorsCleanupTransaction<Transaction>;
	extern template class TReplicatorsCleanupTransaction<EmbeddedTransaction>;

	using ReplicatorsCleanupTransaction = TReplicatorsCleanupTransaction<Transaction>;
	using EmbeddedReplicatorsCleanupTransaction = TReplicatorsCleanupTransaction<EmbeddedTransaction>;

	/// Creates replicators cleanup transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<ReplicatorsCleanupTransaction>
	CreateReplicatorsCleanupTransaction(
		const std::vector<Key>& replicatorKeys,
		std::optional<Amount> maxFee = std::nullopt,
		std::optional<NetworkDuration> deadline = std::nullopt,
		std::optional<NetworkIdentifier> networkId = std::nullopt);


	/// Creates embedded replicators cleanup transaction.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedReplicatorsCleanupTransaction>
	CreateEmbeddedReplicatorsCleanupTransaction(
		const std::vector<Key>& replicatorKeys,
		const Key& signer,
		std::optional<NetworkIdentifier> networkId = std::nullopt);
}
