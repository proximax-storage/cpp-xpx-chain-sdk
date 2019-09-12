/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/mosaic_supply_change_transaction.h>

namespace xpx_chain_sdk {
	
	template<typename TBase>
	MosaicId TMosaicSupplyChangeTransaction<TBase>::mosaicId() const
	{
		return mosaicId_;
	}
	
	template<typename TBase>
	MosaicSupplyChangeDirection TMosaicSupplyChangeTransaction<TBase>::changeDirection() const
	{
		return changeDirection_;
	}
	
	template<typename TBase>
	Amount TMosaicSupplyChangeTransaction<TBase>::changeDelta() const
	{
		return changeDelta_;
	}
	
	template class TMosaicSupplyChangeTransaction<Transaction>;
	template class TMosaicSupplyChangeTransaction<EmbeddedTransaction>;
}
