
#include <nemcpp/model/transaction/modify_multisig_account_transaction.h>

namespace xpx_sdk {
	
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
