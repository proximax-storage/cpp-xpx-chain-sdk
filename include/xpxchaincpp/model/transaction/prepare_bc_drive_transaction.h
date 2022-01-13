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
	
	/// Prepare drive transaction base class.
	template<typename TBase>
	class TPrepareBcDriveTransaction: public TBase {
	public:
		/// Creates prepare drive transaction.
		template<typename... TArgs>
		explicit TPrepareBcDriveTransaction(
				uint64_t driveSize,
				uint16_t replicatorCount,
		        TArgs&&... args):
			TBase(TransactionType::Prepare_Bc_Drive, std::forward<TArgs>(args)...),
			driveSize_(driveSize),
			replicatorCount_(replicatorCount)
		{ }
		
		/// Returns drive size.
		uint64_t driveSize() const;

		/// Returns replicator count.
		uint16_t replicatorCount() const;
		
	private:
		uint64_t driveSize_;
		uint16_t replicatorCount_;
	};
	
	extern template class TPrepareBcDriveTransaction<Transaction>;
	extern template class TPrepareBcDriveTransaction<EmbeddedTransaction>;
	
	using PrepareBcDriveTransaction = TPrepareBcDriveTransaction<Transaction>;
	using EmbeddedPrepareBcDriveTransaction = TPrepareBcDriveTransaction<EmbeddedTransaction>;
	
	/// Creates prepare drive transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<PrepareBcDriveTransaction>
    CreatePrepareBcDriveTransaction(uint64_t driveSize,
                                    uint16_t replicatorCount,
                                    std::optional<Amount> maxFee = std::nullopt,
                                    std::optional<NetworkDuration> deadline = std::nullopt,
                                    std::optional<NetworkIdentifier> networkId = std::nullopt);
	
	
	/// Creates prepare drive transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedPrepareBcDriveTransaction>
	CreateEmbeddedPrepareBcDriveTransaction(uint64_t driveSize,
	                                  uint16_t replicatorCount,
	                                  const Key& signer,
	                                  std::optional<NetworkIdentifier> networkId = std::nullopt);
}
