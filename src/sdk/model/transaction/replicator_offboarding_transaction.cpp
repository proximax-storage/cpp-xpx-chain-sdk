/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/replicator_offboarding_transaction.h>

namespace xpx_chain_sdk {

	template<typename TBase>
	const Key& TReplicatorOffboardingTransaction<TBase>::driveKey() const
	{
		return driveKey_;
	}

	template class TReplicatorOffboardingTransaction<Transaction>;
	template class TReplicatorOffboardingTransaction<EmbeddedTransaction>;
}