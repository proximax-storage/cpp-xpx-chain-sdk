/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/data_modification_approval_transaction.h>

namespace xpx_chain_sdk {

	template<typename TBase>
	const Key& TDataModificationApprovalTransaction<TBase>::driveKey() const
	{
		return driveKey_;
	}

	template<typename TBase>
	const Hash256& TDataModificationApprovalTransaction<TBase>::dataModificationId() const
	{
		return dataModificationId_;
	}

	template<typename TBase>
	const Hash256& TDataModificationApprovalTransaction<TBase>::fileStructureCdi() const
	{
		return fileStructureCdi_;
	}

	template<typename TBase>
	uint64_t TDataModificationApprovalTransaction<TBase>::fileStructureSize() const
	{
		return fileStructureSize_;
	}

	template<typename TBase>
	uint64_t TDataModificationApprovalTransaction<TBase>::usedDriveSize() const
	{
		return usedDriveSize_;
	}

	template class TDataModificationApprovalTransaction<Transaction>;
	template class TDataModificationApprovalTransaction<EmbeddedTransaction>;
}
