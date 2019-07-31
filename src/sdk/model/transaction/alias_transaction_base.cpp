
#include <nemcpp/model/transaction/alias_transaction_base.h>

namespace xpx_sdk {
	
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
