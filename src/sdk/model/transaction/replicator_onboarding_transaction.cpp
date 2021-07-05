/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/replicator_onboarding_transaction.h>

namespace xpx_chain_sdk {

	template<typename TBase>
	const Amount& TReplicatorOnboardingTransaction<TBase>::capacity() const
	{
		return capacity_;
	}

	template class TReplicatorOnboardingTransaction<Transaction>;
	template class TReplicatorOnboardingTransaction<EmbeddedTransaction>;
}
