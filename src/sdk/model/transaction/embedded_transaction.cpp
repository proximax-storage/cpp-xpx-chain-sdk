
#include <xpxchaincpp/model/transaction/embedded_transaction.h>

namespace xpx_chain_sdk {
	
	EmbeddedTransaction::EmbeddedTransaction(TransactionType type, uint16_t fullVersion, const Key& signer):
		BasicTransaction(type, fullVersion, signer)
	{ }
	
	bool EmbeddedTransaction::isEmbedded() const
	{
		return true;
	}
}
