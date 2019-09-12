/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/modify_multisig_account_transaction.h>

namespace xpx_chain_sdk {
	
	template<typename TBase>
	int8_t TModifyMultisigAccountTransaction<TBase>::minRemovalDelta() const
	{
		return minRemovalDelta_;
	}
	
	template<typename TBase>
	int8_t TModifyMultisigAccountTransaction<TBase>::minApprovalDelta() const
	{
		return minApprovalDelta_;
	}
	
	template<typename TBase>
	const CosignatoryModifications& TModifyMultisigAccountTransaction<TBase>::cosignatoryModifications() const
	{
		return cosignatoryModifications_;
	}
	
	template class TModifyMultisigAccountTransaction<Transaction>;
	template class TModifyMultisigAccountTransaction<EmbeddedTransaction>;
}
