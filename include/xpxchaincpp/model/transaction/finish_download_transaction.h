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

	/// Finish download transaction base class.
	template<typename TBase>
	class TFinishDownloadTransaction: public TBase {
	public:
		/// Creates finish download transaction.
		template<typename... TArgs>
		explicit TFinishDownloadTransaction(
				const Hash256& downloadChannelId,
				const Amount& feedbackFeeAmount,
		        TArgs&&... args):
			TBase(TransactionType::Finish_Download, std::forward<TArgs>(args)...),
            downloadChannelId_(downloadChannelId),
            feedbackFeeAmount_(feedbackFeeAmount)
		{ }

		/// Returns identifier of the download channel.
		const Hash256& downloadChannelId() const;

		/// Returns amount of XPXs to transfer to the download channel.
		const Amount& feedbackFeeAmount() const;

	private:
        Hash256 downloadChannelId_;
        Amount feedbackFeeAmount_;
	};

	extern template class TFinishDownloadTransaction<Transaction>;
	extern template class TFinishDownloadTransaction<EmbeddedTransaction>;

	using FinishDownloadTransaction = TFinishDownloadTransaction<Transaction>;
	using EmbeddedFinishDownloadTransaction = TFinishDownloadTransaction<EmbeddedTransaction>;

	/// Creates finish download transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
	std::unique_ptr<FinishDownloadTransaction>
    CreateFinishDownloadTransaction(const Hash256& downloadChannelId,
                                    const Amount& feedbackFeeAmount,
                                    std::optional<Amount> maxFee = std::nullopt,
                                    std::optional<NetworkDuration> deadline = std::nullopt,
                                    std::optional<NetworkIdentifier> networkId = std::nullopt);


	/// Creates finish download transaction.
	/// \note Throws \c transaction_error if mosaics or message have invalid size.
	/// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
    std::unique_ptr<EmbeddedFinishDownloadTransaction>
    CreateEmbeddedFinishDownloadTransaction(const Hash256& downloadChannelId,
                                            const Amount& feedbackFeeAmount,
                                            const Key& signer,
                                            std::optional<NetworkIdentifier> networkId = std::nullopt);
}
