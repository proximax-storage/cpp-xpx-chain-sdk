
#include <xpxchaincpp/model/transaction/lock_funds_transaction.h>

namespace xpx_chain_sdk {
	
	template<typename TBase>
	const Mosaic& TLockFundsTransaction<TBase>::lockedMosaic() const
	{
		return lockedMosaic_;
	}
	
	template<typename TBase>
	BlockDuration TLockFundsTransaction<TBase>::lockDuration() const
	{
		return lockDuration_;
	}
	
	template<typename TBase>
	const Hash256& TLockFundsTransaction<TBase>::lockHash() const
	{
		return lockHash_;
	}
	
	template class TLockFundsTransaction<Transaction>;
	template class TLockFundsTransaction<EmbeddedTransaction>;
}
