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

	/// Data modification cancel transaction base class.
	template<typename TBase>
	class TDataModificationCancelTransaction: public TBase {
	public:
		/// Creates data modification cancel transaction.
		template<typename... TArgs>
		explicit TDataModificationCancelTransaction(
				const Key& driveKey,
				const Hash256& dataModificationId,
		        TArgs&&... args):
			TBase(TransactionType::Data_Modification_Approval, std::forward<TArgs>(args)...),
			driveKey_(driveKey),
			dataModificationId_(dataModificationId)
		{ }

		/// Returns drive key.
		const Key& driveKey() const;

		/// Returns identifier of the transaction that initiated the modification.
		const Hash256& dataModificationId() const;

	private:
		Key driveKey_;
		Hash256 dataModificationId_;
	};

	extern template class TDataModificationCancelTransaction<Transaction>;
	extern template class TDataModificationCancelTransaction<EmbeddedTransaction>;

	using DataModificationCancelTransaction = TDataModificationCancelTransaction<Transaction>;
	using EmbeddedDataModificationCancelTransaction = TDataModificationCancelTransaction<EmbeddedTransaction>;

	/// Creates data modification cancel transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<DataModificationCancelTransaction>
	CreateDataModificationCancelTransaction(const Key& driveKey,
	                          const Hash256& dataModificationId,
	                          std::optional<Amount> maxFee = std::nullopt,
	                          std::optional<NetworkDuration> deadline = std::nullopt,
	                          std::optional<NetworkIdentifier> networkId = std::nullopt);


	/// Creates data modification cancel transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedDataModificationCancelTransaction>
	CreateEmbeddedDataModificationCancelTransaction(const Key& driveKey,
	                                  const Hash256& dataModificationId,
	                                  const Key& signer,
	                                  std::optional<NetworkIdentifier> networkId = std::nullopt);
}
