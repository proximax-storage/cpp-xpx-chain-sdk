/**
*** Copyright 2022 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/create_liquidity_provider_transaction.h>

namespace xpx_chain_sdk {
	
	template<typename TBase>
	MosaicId TCreateLiquidityProviderTransaction<TBase>::providerMosaicId() const
	{
		return providerMosaicId_;
	}

	template<typename TBase>
	Amount TCreateLiquidityProviderTransaction<TBase>::currencyDeposit() const
	{
		return currencyDeposit_;
	}

	template<typename TBase>
	Amount TCreateLiquidityProviderTransaction<TBase>::initialMosaicsMinting() const
	{
		return initialMosaicsMinting_;
	}

	template<typename TBase>
	uint32_t TCreateLiquidityProviderTransaction<TBase>::slashingPeriod() const
	{
		return slashingPeriod_;
	}

	template<typename TBase>
	uint16_t TCreateLiquidityProviderTransaction<TBase>::windowSize() const
	{
		return windowSize_;
	}

    template<typename TBase>
    const Key& TCreateLiquidityProviderTransaction<TBase>::slashingAccount() const
    {
        return slashingAccount_;
    }

    template<typename TBase>
    uint32_t TCreateLiquidityProviderTransaction<TBase>::alpha() const
    {
        return alpha_;
    }

    template<typename TBase>
    uint32_t TCreateLiquidityProviderTransaction<TBase>::beta() const
    {
        return beta_;
    }

	template class TCreateLiquidityProviderTransaction<Transaction>;
	template class TCreateLiquidityProviderTransaction<EmbeddedTransaction>;
}
