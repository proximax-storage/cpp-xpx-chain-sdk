/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/alias_transaction_base.h>

namespace xpx_chain_sdk {
	
	template<typename TBase>
	AliasTransactionAction TAliasTransactionBase<TBase>::action() const
	{
		return action_;
	}
	
	template<typename TBase>
	NamespaceId TAliasTransactionBase<TBase>::aliasNamespaceId() const
	{
		return aliasNamespaceId_;
	}
	
	template class TAliasTransactionBase<Transaction>;
	template class TAliasTransactionBase<EmbeddedTransaction>;
}
