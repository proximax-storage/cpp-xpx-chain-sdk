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

	/// Download transaction base class.
	template<typename TBase>
	class TDownloadTransaction: public TBase {
	public:
		/// Creates download transaction.
		template<typename... TArgs>
		explicit TDownloadTransaction(
				const Key& driveKey,
				uint64_t downloadSize,
				const Amount& feedbackFeeAmount,
				const uint16_t& listOfPublicKeysSize,
                const std::vector<Key>& listOfPublicKeys,
		        TArgs&&... args):
			TBase(TransactionType::Download, std::forward<TArgs>(args)...),
			driveKey_(driveKey),
			downloadSize_(downloadSize),
            feedbackFeeAmount_(feedbackFeeAmount),
            listOfPublicKeysSize_(listOfPublicKeysSize),
            listOfPublicKeys_(listOfPublicKeys)
		{ }

		/// Returns drive key.
		const Key& driveKey() const;

		/// Returns download size.
		uint64_t downloadSize() const;

		/// Returns XPXs to lock for future payment for.
		const Amount& feedbackFeeAmount() const;

        /// Returns size of the list of public keys.
        uint16_t listOfPublicKeysSize() const;

        /// Returns list of public keys.
        std::vector<Key> listOfPublicKeys() const;

	private:
		Key driveKey_;
		uint64_t downloadSize_;
		Amount feedbackFeeAmount_;
        uint16_t listOfPublicKeysSize_;
        std::vector<Key> listOfPublicKeys_;
	};

	extern template class TDownloadTransaction<Transaction>;
	extern template class TDownloadTransaction<EmbeddedTransaction>;

	using DownloadTransaction = TDownloadTransaction<Transaction>;
	using EmbeddedDownloadTransaction = TDownloadTransaction<EmbeddedTransaction>;

	/// Creates download transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<DownloadTransaction>
	CreateDownloadTransaction(const Key& driveKey,
	                          uint64_t downloadSize,
                              const Amount& feedbackFeeAmount,
                              const uint16_t& listOfPublicKeysSize,
                              const std::vector<Key>& listOfPublicKeys,
	                          std::optional<Amount> maxFee = std::nullopt,
	                          std::optional<NetworkDuration> deadline = std::nullopt,
	                          std::optional<NetworkIdentifier> networkId = std::nullopt);


	/// Creates download transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<EmbeddedDownloadTransaction>
	CreateEmbeddedDownloadTransaction(const Key& driveKey,
	                                  uint64_t downloadSize,
                                      const Amount& feedbackFeeAmount,
                                      const uint16_t& listOfPublicKeysSize,
                                      const std::vector<Key>& listOfPublicKeys,
	                                  const Key& signer,
	                                  std::optional<NetworkIdentifier> networkId = std::nullopt);
}
