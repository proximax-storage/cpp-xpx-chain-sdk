/**
*** Copyright 2022 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/manual_rate_change_transaction.h>

namespace xpx_chain_sdk {
	
	template<typename TBase>
	MosaicId TManualRateChangeTransaction<TBase>::providerMosaicId() const
	{
		return providerMosaicId_;
	}

	template<typename TBase>
	bool TManualRateChangeTransaction<TBase>::currencyBalanceIncrease() const
	{
		return currencyBalanceIncrease_;
	}

	template<typename TBase>
	Amount TManualRateChangeTransaction<TBase>::currencyBalanceChange() const
	{
		return currencyBalanceChange_;
	}

	template<typename TBase>
	bool TManualRateChangeTransaction<TBase>::mosaicBalanceIncrease() const
	{
		return mosaicBalanceIncrease_;
	}

	template<typename TBase>
	Amount TManualRateChangeTransaction<TBase>::mosaicBalanceChange() const
	{
		return mosaicBalanceChange_;
	}

	template class TManualRateChangeTransaction<Transaction>;
	template class TManualRateChangeTransaction<EmbeddedTransaction>;
}
