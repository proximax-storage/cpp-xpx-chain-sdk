/**
*** Copyright 2022 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/model/account/address.h>
#include <xpxchaincpp/model/mosaic/mosaic.h>
#include <xpxchaincpp/model/transaction/transaction.h>
#include <xpxchaincpp/model/transaction/embedded_transaction.h>

#include <memory>
#include <optional>
#include <string>
#include <utility>

namespace xpx_chain_sdk {

    /// Manual rate change transaction base class.
    template<typename TBase>
    class TManualRateChangeTransaction: public TBase {
    public:
        /// Creates manual rate change transaction.
        template<typename... TArgs>
        explicit TManualRateChangeTransaction(const MosaicId providerMosaicId,
                                              const bool currencyBalanceIncrease,
                                              const Amount currencyBalanceChange,
                                              const bool mosaicBalanceIncrease,
                                              const Amount mosaicBalanceChange,
                                              TArgs&&... args):
                TBase(TransactionType::Manual_Rate_Change, std::forward<TArgs>(args)...),
                providerMosaicId_(providerMosaicId),
                currencyBalanceIncrease_(currencyBalanceIncrease),
                currencyBalanceChange_(currencyBalanceChange),
                mosaicBalanceIncrease_(mosaicBalanceIncrease),
                mosaicBalanceChange_(mosaicBalanceChange)
        { }
		
		/// Returns mosaic id.
		MosaicId providerMosaicId() const;
		
		/// Returns currency balance increase.
		bool currencyBalanceIncrease() const;
		
		/// Returns currency balance change amount.
		Amount currencyBalanceChange() const;
		
		/// Returns mosaic balance increase.
		bool mosaicBalanceIncrease() const;
		
		/// Returns mosaic balance change amount.
        Amount mosaicBalanceChange() const;
		
	private:
		MosaicId providerMosaicId_;
        bool currencyBalanceIncrease_;
        Amount currencyBalanceChange_;
        bool mosaicBalanceIncrease_;
        Amount mosaicBalanceChange_;
	};
	
	extern template class TManualRateChangeTransaction<Transaction>;
	extern template class TManualRateChangeTransaction<EmbeddedTransaction>;
	
	using ManualRateChangeTransaction = TManualRateChangeTransaction<Transaction>;
	using EmbeddedManualRateChangeTransaction = TManualRateChangeTransaction<EmbeddedTransaction>;

    /// Creates manual rate change transaction.
    /// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
    std::unique_ptr<ManualRateChangeTransaction>
    CreateManualRateChangeTransaction(const MosaicId providerMosaicId,
                                      const bool currencyBalanceIncrease,
                                      const Amount currencyBalanceChange,
                                      const bool mosaicBalanceIncrease,
                                      const Amount mosaicBalanceChange,
                                      std::optional<Amount> maxFee = std::nullopt,
                                      std::optional<NetworkDuration> deadline = std::nullopt,
                                      std::optional<NetworkIdentifier> networkId = std::nullopt);


    /// Creates manual rate change transaction.
    /// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
    std::unique_ptr<EmbeddedManualRateChangeTransaction>
    CreateEmbeddedManualRateChangeTransaction(const MosaicId providerMosaicId,
                                              const bool currencyBalanceIncrease,
                                              const Amount currencyBalanceChange,
                                              const bool mosaicBalanceIncrease,
                                              const Amount mosaicBalanceChange,
                                              const Key& signer,
                                              std::optional<NetworkIdentifier> networkId = std::nullopt);
}
