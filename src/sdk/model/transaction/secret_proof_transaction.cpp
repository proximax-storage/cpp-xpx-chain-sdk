
#include <xpxchaincpp/model/transaction/secret_proof_transaction.h>

namespace xpx_chain_sdk {
	
	template<typename TBase>
	SecretHashAlgorithm TSecretProofTransaction<TBase>::secretHashAlgorithm() const
	{
		return secretHashAlgorithm_;
	}

	template<typename TBase>
	const Hash256& TSecretProofTransaction<TBase>::secretHash() const
	{
		return secretHash_;
	}

	template<typename TBase>
	const RawBuffer& TSecretProofTransaction<TBase>::secret() const
	{
		// We do not return new buffer object in each call because we would like to allow usage like
		// 'for_each(tx.message().begin(), tx.message().end() ...)' and returning new object from this
		// method can lead to unexpected errors in the future because 'begin' and 'end' will be called
		// on different buffer objects (though it's currently will work).
		return secretBuffer_;
	}

	template class TSecretProofTransaction<Transaction>;
	template class TSecretProofTransaction<EmbeddedTransaction>;
}
