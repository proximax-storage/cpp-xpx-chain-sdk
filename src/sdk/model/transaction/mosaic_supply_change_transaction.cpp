
#include <nemcpp/model/transaction/mosaic_supply_change_transaction.h>

namespace xpx_sdk {
	
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
