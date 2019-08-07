
#include <nemcpp/model/transaction/mosaic_alias_transaction.h>

namespace xpx_sdk::difficult_transactions {
	
	template<typename TBase>
	MosaicId TMosaicAliasTransaction<TBase>::aliasedMosaicId() const
	{
		return aliasedMosaicId_;
	}
	
	template class TMosaicAliasTransaction<Transaction>;
	template class TMosaicAliasTransaction<EmbeddedTransaction>;
}
