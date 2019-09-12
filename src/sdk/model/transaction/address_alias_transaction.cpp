
#include <xpxchaincpp/model/transaction/address_alias_transaction.h>

namespace xpx_chain_sdk {
	
	template<typename TBase>
	const Address& TAddressAliasTransaction<TBase>::aliasedAddress() const
	{
		return aliasedAddress_;
	}

	template class TAddressAliasTransaction<Transaction>;
	template class TAddressAliasTransaction<EmbeddedTransaction>;
}
