
#include <nemcpp/model/transaction/address_alias_transaction.h>

namespace xpx_sdk::difficult_transactions {
	
	template<typename TBase>
	const Address& TAddressAliasTransaction<TBase>::aliasedAddress() const
	{
		return aliasedAddress_;
	}

	template class TAddressAliasTransaction<Transaction>;
	template class TAddressAliasTransaction<EmbeddedTransaction>;
}
