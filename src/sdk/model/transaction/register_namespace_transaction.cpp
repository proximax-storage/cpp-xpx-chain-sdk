
#include <nemcpp/model/transaction/register_namespace_transaction.h>

namespace xpx_sdk {
	
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
