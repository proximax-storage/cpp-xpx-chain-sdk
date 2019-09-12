/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/mosaic_alias_transaction.h>

namespace xpx_chain_sdk {
	
	template<typename TBase>
	MosaicId TMosaicAliasTransaction<TBase>::aliasedMosaicId() const
	{
		return aliasedMosaicId_;
	}
	
	template class TMosaicAliasTransaction<Transaction>;
	template class TMosaicAliasTransaction<EmbeddedTransaction>;
}
