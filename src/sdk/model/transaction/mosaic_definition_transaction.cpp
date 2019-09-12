
#include <nemcpp/model/transaction/mosaic_definition_transaction.h>

namespace xpx_sdk {
	
	template<typename TBase>
	uint32_t TMosaicDefinitionTransaction<TBase>::mosaicNonce() const
	{
		return mosaicNonce_;
	}
	
	template<typename TBase>
	MosaicId TMosaicDefinitionTransaction<TBase>::mosaicId() const
	{
		return mosaicId_;
	}

	template<typename TBase>
	MosaicFlags TMosaicDefinitionTransaction<TBase>::flags() const
	{
		return flags_;
	}
	
	template<typename TBase>
	const MosaicProperties& TMosaicDefinitionTransaction<TBase>::mosaicProperties() const
	{
		return mosaicProperties_;
	}
	
	template class TMosaicDefinitionTransaction<Transaction>;
	template class TMosaicDefinitionTransaction<EmbeddedTransaction>;
}
