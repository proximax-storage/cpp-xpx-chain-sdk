
#include <nemcpp/model/transaction/account_link_transaction.h>

namespace nem2_sdk {
	
	template<typename TBase>
	AccountLinkTransactionAction TAccountLinkTransaction<TBase>::action() const
	{
		return action_;
	}
	
	template<typename TBase>
	const PublicAccount& TAccountLinkTransaction<TBase>::remotePublicAccount() const
	{
		return remotePublicAccount_;
	}
	
	template class TAccountLinkTransaction<Transaction>;
	template class TAccountLinkTransaction<EmbeddedTransaction>;
}
