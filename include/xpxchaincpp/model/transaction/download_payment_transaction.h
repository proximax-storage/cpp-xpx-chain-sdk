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

	/// Download payment transaction base class.
	template<typename TBase>
	class TDownloadPaymentTransaction: public TBase {
	public:
		/// Creates download payment transaction.
		template<typename... TArgs>
		explicit TDownloadPaymentTransaction(
				const Hash256& downloadChannelId,
				uint64_t downloadSize,
				const Amount& feedbackFeeAmount,
		        TArgs&&... args):
			TBase(TransactionType::Download_Payment, std::forward<TArgs>(args)...),
            downloadChannelId_(downloadChannelId),
			downloadSize_(downloadSize),
            feedbackFeeAmount_(feedbackFeeAmount)
		{ }

		/// Returns identifier of the download channel.
		const Hash256& downloadChannelId() const;

		/// Returns download size to add to the prepaid size of the download channel.
		uint64_t downloadSize() const;

		/// Returns amount of XPXs to transfer to the download channel.
		const Amount& feedbackFeeAmount() const;

	private:
        Hash256 downloadChannelId_;
		uint64_t downloadSize_;
        Amount feedbackFeeAmount_;
	};

	extern template class TDownloadPaymentTransaction<Transaction>;
	extern template class TDownloadPaymentTransaction<EmbeddedTransaction>;

	using DownloadPaymentTransaction = TDownloadPaymentTransaction<Transaction>;
	using EmbeddedDownloadPaymentTransaction = TDownloadPaymentTransaction<EmbeddedTransaction>;

	/// Creates download payment transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<DownloadPaymentTransaction>
    CreateDownloadPaymentTransaction(const Hash256& downloadChannelId,
                                     uint64_t downloadSize,
                                     const Amount& feedbackFeeAmount,
                                     std::optional<Amount> maxFee = std::nullopt,
                                     std::optional<NetworkDuration> deadline = std::nullopt,
                                     std::optional<NetworkIdentifier> networkId = std::nullopt);


	/// Creates download payment transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
    std::unique_ptr<EmbeddedDownloadPaymentTransaction>
    CreateEmbeddedDownloadPaymentTransaction(const Hash256& downloadChannelId,
                                             uint64_t downloadSize,
                                             const Amount& feedbackFeeAmount,
                                             const Key& signer,
                                             std::optional<NetworkIdentifier> networkId = std::nullopt);
}
