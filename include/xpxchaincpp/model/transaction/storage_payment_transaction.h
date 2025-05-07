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

	/// Storage payment transaction base class.
	template<typename TBase>
	class TStoragePaymentTransaction: public TBase {
	public:
		/// Creates storage payment transaction.
		template<typename... TArgs>
		explicit TStoragePaymentTransaction(
				const Key& driveKey,
				const Amount& storageUnits,
		        TArgs&&... args):
			TBase(TransactionType::Storage_Payment, std::forward<TArgs>(args)...),
            driveKey_(driveKey),
            storageUnits_(storageUnits)
		{ }

		/// Returns identifier of the drive.
		const Key& driveKey() const;

		/// Returns amount of storage units to transfer to the drive.
		const Amount& storageUnitsAmount() const;

	private:
        Key driveKey_;
        Amount storageUnits_;
	};

	extern template class TStoragePaymentTransaction<Transaction>;
	extern template class TStoragePaymentTransaction<EmbeddedTransaction>;

	using StoragePaymentTransaction = TStoragePaymentTransaction<Transaction>;
	using EmbeddedStoragePaymentTransaction = TStoragePaymentTransaction<EmbeddedTransaction>;

	/// Creates storage payment transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<StoragePaymentTransaction>
    CreateStoragePaymentTransaction(const Key& driveKey,
                                    const Amount& storageUnits,
                                    std::optional<Amount> maxFee = std::nullopt,
                                    std::optional<NetworkDuration> deadline = std::nullopt,
                                    std::optional<NetworkIdentifier> networkId = std::nullopt);


	/// Creates storage payment transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
    std::unique_ptr<EmbeddedStoragePaymentTransaction>
    CreateEmbeddedStoragePaymentTransaction(const Key& driveKey,
                                            const Amount& storageUnits,
                                            const Key& signer,
                                            std::optional<NetworkIdentifier> networkId = std::nullopt);
}
