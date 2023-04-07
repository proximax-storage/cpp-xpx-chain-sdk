/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/data_modification_transaction.h>

namespace xpx_chain_sdk {
	
	template<typename TBase>
	const Key& TDataModificationTransaction<TBase>::driveKey() const
	{
		return driveKey_;
	}

	template<typename TBase>
	const Hash256& TDataModificationTransaction<TBase>::downloadDataCdi() const
	{
		return downloadDataCdi_;
	}

	template<typename TBase>
	uint64_t TDataModificationTransaction<TBase>::uploadSize() const
	{
		return uploadSize_;
	}

    template<typename TBase>
    Amount TDataModificationTransaction<TBase>::feedbackFeeAmount() const
    {
        return feedbackFeeAmount_;
    }

	template class TDataModificationTransaction<Transaction>;
	template class TDataModificationTransaction<EmbeddedTransaction>;
}
