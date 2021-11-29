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

	/// Drive closure transaction base class.
	template<typename TBase>
	class TDriveClosureTransaction: public TBase {
	public:
		/// Creates drive closure transaction.
		template<typename... TArgs>
		explicit TDriveClosureTransaction(
				const Key& driveKey,
		        TArgs&&... args):
			TBase(TransactionType::Drive_Closure, std::forward<TArgs>(args)...),
			driveKey_(driveKey)
		{ }

		/// Returns drivekey that the replicator is willing to contribute.
		const Key& driveKey() const;

	private:
		Key driveKey_;
	};

	extern template class TDriveClosureTransaction<Transaction>;
	extern template class TDriveClosureTransaction<EmbeddedTransaction>;

	using DriveClosureTransaction = TDriveClosureTransaction<Transaction>;
	using EmbeddedDriveClosureTransaction = TDriveClosureTransaction<EmbeddedTransaction>;

	/// Creates drive closure transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<DriveClosureTransaction>
	CreateDriveClosureTransaction(const Key& driveKey,
	                          std::optional<Amount> maxFee = std::nullopt,
	                          std::optional<NetworkDuration> deadline = std::nullopt,
	                          std::optional<NetworkIdentifier> networkId = std::nullopt);


	/// Creates drive closure transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedDriveClosureTransaction>
	CreateEmbeddedDriveClosureTransaction(const Key& driveKey,
	                                  const Key& signer,
	                                  std::optional<NetworkIdentifier> networkId = std::nullopt);
}
