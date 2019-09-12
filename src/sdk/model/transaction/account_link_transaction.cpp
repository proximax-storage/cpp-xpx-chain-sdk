/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/account_link_transaction.h>

namespace xpx_chain_sdk {
	
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
