/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/data_modification_cancel_transaction.h>

namespace xpx_chain_sdk {

	template<typename TBase>
	const Key& TDataModificationCancelTransaction<TBase>::driveKey() const
	{
		return driveKey_;
	}

	template<typename TBase>
	const Hash256& TDataModificationCancelTransaction<TBase>::dataModificationId() const
	{
		return dataModificationId_;
	}

	template class TDataModificationCancelTransaction<Transaction>;
	template class TDataModificationCancelTransaction<EmbeddedTransaction>;
}
