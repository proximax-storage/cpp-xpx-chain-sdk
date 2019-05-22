
#include <nemcpp/model/transaction/mosaic_alias_transaction.h>

namespace nem2_sdk {
	
	template<typename TBase>
	MosaicId TMosaicAliasTransaction<TBase>::aliasedMosaicId() const
	{
		return aliasedMosaicId_;
	}
	
	template class TMosaicAliasTransaction<Transaction>;
	template class TMosaicAliasTransaction<EmbeddedTransaction>;
}
