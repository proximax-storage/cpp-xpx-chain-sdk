/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/secret_lock_transaction.h>

namespace xpx_chain_sdk {
	
	template<typename TBase>
	const Hash256& TSecretLockTransaction<TBase>::secretHash() const
	{
		return secretHash_;
	}

	template<typename TBase>
	SecretHashAlgorithm TSecretLockTransaction<TBase>::secretHashAlgorithm() const
	{
		return secretHashAlgorithm_;
	}

	template<typename TBase>
	const Mosaic& TSecretLockTransaction<TBase>::lockedMosaic() const
	{
		return lockedMosaic_;
	}

	template<typename TBase>
	BlockDuration TSecretLockTransaction<TBase>::lockDuration() const
	{
		return lockDuration_;
	}

	template<typename TBase>
	const Address& TSecretLockTransaction<TBase>::lockedMosaicRecipient() const
	{
		return lockedMosaicRecipient_;
	}

	template class TSecretLockTransaction<Transaction>;
	template class TSecretLockTransaction<EmbeddedTransaction>;
}
