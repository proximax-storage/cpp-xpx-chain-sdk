/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/prepare_bc_drive_transaction.h>

namespace xpx_chain_sdk {
	
	template<typename TBase>
	uint64_t TPrepareBcDriveTransaction<TBase>::driveSize() const
	{
		return driveSize_;
	}

	template<typename TBase>
	uint16_t TPrepareBcDriveTransaction<TBase>::replicatorCount() const
	{
		return replicatorCount_;
	}

	template class TPrepareBcDriveTransaction<Transaction>;
	template class TPrepareBcDriveTransaction<EmbeddedTransaction>;
}
