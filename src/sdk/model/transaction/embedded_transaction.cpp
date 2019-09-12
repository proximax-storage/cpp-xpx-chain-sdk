/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
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
