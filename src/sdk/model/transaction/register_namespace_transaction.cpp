/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/register_namespace_transaction.h>

namespace xpx_chain_sdk {
	
	template<typename TBase>
	const Namespace& TRegisterNamespaceTransaction<TBase>::ns() const
	{
		return namespace_;
	}

	template<typename TBase>
	BlockDuration TRegisterNamespaceTransaction<TBase>::nsDuration() const
	{
		return namespaceDuration_;
	}

	template class TRegisterNamespaceTransaction<Transaction>;
	template class TRegisterNamespaceTransaction<EmbeddedTransaction>;
}
