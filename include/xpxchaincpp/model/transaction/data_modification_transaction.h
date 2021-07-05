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

	/// Data modification transaction base class.
	template<typename TBase>
	class TDataModificationTransaction: public TBase {
	public:
		/// Creates data modification transaction.
		template<typename... TArgs>
		explicit TDataModificationTransaction(
				const Key& driveKey,
				const Hash256& downloadDataCdi,
				uint64_t uploadSize,
		        TArgs&&... args):
			TBase(TransactionType::Data_Modification, std::forward<TArgs>(args)...),
			driveKey_(driveKey),
			downloadDataCdi_(downloadDataCdi),
			uploadSize_(uploadSize)
		{ }

		/// Returns drive key.
		const Key& driveKey() const;

		/// Returns download data CDI (Content Download Info).
		const Hash256& downloadDataCdi() const;

		/// Returns upload size.
		uint64_t uploadSize() const;

	private:
		Key driveKey_;
		Hash256 downloadDataCdi_;
		uint64_t uploadSize_;
	};

	extern template class TDataModificationTransaction<Transaction>;
	extern template class TDataModificationTransaction<EmbeddedTransaction>;

	using DataModificationTransaction = TDataModificationTransaction<Transaction>;
	using EmbeddedDataModificationTransaction = TDataModificationTransaction<EmbeddedTransaction>;

	/// Creates data modification transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<DataModificationTransaction>
	CreateDataModificationTransaction(const Key& driveKey,
	                          const Hash256& downloadDataCdi,
	                          uint64_t uploadSize,
	                          std::optional<Amount> maxFee = std::nullopt,
	                          std::optional<NetworkDuration> deadline = std::nullopt,
	                          std::optional<NetworkIdentifier> networkId = std::nullopt);


	/// Creates data modification transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedDataModificationTransaction>
	CreateEmbeddedDataModificationTransaction(const Key& driveKey,
	                                  const Hash256& downloadDataCdi,
	                                  uint64_t uploadSize,
	                                  const Key& signer,
	                                  std::optional<NetworkIdentifier> networkId = std::nullopt);
}
