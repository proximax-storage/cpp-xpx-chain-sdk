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

    /// Create liquidity provider transaction base class.
    template<typename TBase>
    class TCreateLiquidityProviderTransaction: public TBase {
    public:
        /// Creates create liquidity provider transaction.
        template<typename... TArgs>
        explicit TCreateLiquidityProviderTransaction(const MosaicId providerMosaicId,
                                                     const Amount currencyDeposit,
                                                     const Amount initialMosaicsMinting,
                                                     const uint32_t slashingPeriod,
                                                     const uint16_t windowSize,
                                                     const Key& slashingAccount,
                                                     const uint32_t alpha,
                                                     const uint32_t beta,
                                                     TArgs&&... args):
                TBase(TransactionType::Create_Liquidity_Provider, std::forward<TArgs>(args)...),
                providerMosaicId_(providerMosaicId),
                currencyDeposit_(currencyDeposit),
                initialMosaicsMinting_(initialMosaicsMinting),
                slashingPeriod_(slashingPeriod),
                windowSize_(windowSize),
                slashingAccount_(slashingAccount),
                alpha_(alpha),
                beta_(beta)
        { }
		
		/// Returns mosaic id.
		MosaicId providerMosaicId() const;
		
		/// Returns current deposit.
		Amount currencyDeposit() const;
		
		/// Returns initial mosaics minting.
		Amount initialMosaicsMinting() const;
		
		/// Returns slashing period.
		uint32_t slashingPeriod() const;
		
		/// Returns windows size.
        uint16_t windowSize() const;

        /// Returns slashing account.
        const Key& slashingAccount() const;

        /// Returns alpha.
        uint32_t alpha() const;

        /// Returns beta.
        uint32_t beta() const;
		
	private:
		MosaicId providerMosaicId_;
        Amount currencyDeposit_;
        Amount initialMosaicsMinting_;
        uint32_t slashingPeriod_;
        uint16_t windowSize_;
        Key slashingAccount_;
        uint32_t alpha_;
        uint32_t beta_;
	};
	
	extern template class TCreateLiquidityProviderTransaction<Transaction>;
	extern template class TCreateLiquidityProviderTransaction<EmbeddedTransaction>;
	
	using CreateLiquidityProviderTransaction = TCreateLiquidityProviderTransaction<Transaction>;
	using EmbeddedCreateLiquidityProviderTransaction = TCreateLiquidityProviderTransaction<EmbeddedTransaction>;

    /// Creates liquidity provider transaction.
    /// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
    std::unique_ptr<CreateLiquidityProviderTransaction>
    CreateCreateLiquidityProviderTransaction(const MosaicId providerMosaicId,
                                             const Amount currencyDeposit,
                                             const Amount initialMosaicsMinting,
                                             const uint32_t slashingPeriod,
                                             const uint16_t windowSize,
                                             const Key& slashingAccount,
                                             const uint32_t alpha,
                                             const uint32_t beta,
                                             std::optional<Amount> maxFee = std::nullopt,
                                             std::optional<NetworkDuration> deadline = std::nullopt,
                                             std::optional<NetworkIdentifier> networkId = std::nullopt);


    /// Creates embedded liquidity provider transaction.
    /// \note Optional transaction parameters are initialized using \c Config if not set explicitly.
    std::unique_ptr<EmbeddedCreateLiquidityProviderTransaction>
    CreateEmbeddedCreateLiquidityProviderTransaction(const MosaicId providerMosaicId,
                                                     const Amount currencyDeposit,
                                                     const Amount initialMosaicsMinting,
                                                     const uint32_t slashingPeriod,
                                                     const uint16_t windowSize,
                                                     const Key& slashingAccount,
                                                     const uint32_t alpha,
                                                     const uint32_t beta,
                                                     const Key& signer,
                                                     std::optional<NetworkIdentifier> networkId = std::nullopt);
}
