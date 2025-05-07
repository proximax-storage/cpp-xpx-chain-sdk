/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/storage_payment_transaction.h>

namespace xpx_chain_sdk {
	
	template<typename TBase>
	const Key& TStoragePaymentTransaction<TBase>::driveKey() const
	{
		return driveKey_;
	}

	template<typename TBase>
	const Amount& TStoragePaymentTransaction<TBase>::storageUnitsAmount() const
	{
		return storageUnits_;
	}

	template class TStoragePaymentTransaction<Transaction>;
	template class TStoragePaymentTransaction<EmbeddedTransaction>;
}
