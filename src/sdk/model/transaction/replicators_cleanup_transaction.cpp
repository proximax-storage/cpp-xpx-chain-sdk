/**
*** Copyright 2024 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/replicators_cleanup_transaction.h>

namespace xpx_chain_sdk {

	template<typename TBase>
	const std::vector<Key>& TReplicatorsCleanupTransaction<TBase>::replicatorKeys() const
	{
		return replicatorKeys_;
	}

	template class TReplicatorsCleanupTransaction<Transaction>;
	template class TReplicatorsCleanupTransaction<EmbeddedTransaction>;
}
