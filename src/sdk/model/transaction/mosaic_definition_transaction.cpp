/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/mosaic_definition_transaction.h>

namespace xpx_chain_sdk {
	
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
