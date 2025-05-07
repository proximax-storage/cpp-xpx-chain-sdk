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
    uint8_t TDataModificationApprovalTransaction<TBase>::modificationStatus() const
    {
        return modificationStatus_;
    }

	template<typename TBase>
	uint64_t TDataModificationApprovalTransaction<TBase>::fileStructureSizeBytes() const
	{
		return fileStructureSize_;
	}

    template<typename TBase>
    uint64_t TDataModificationApprovalTransaction<TBase>::metaFilesSizeBytes() const
    {
        return metaFilesSize_;
    }

	template<typename TBase>
	uint64_t TDataModificationApprovalTransaction<TBase>::usedDriveSizeBytes() const
	{
		return usedDriveSize_;
	}

    template<typename TBase>
    uint8_t TDataModificationApprovalTransaction<TBase>::judgingKeysCount() const
    {
        return judgingKeysCount_;
    }

    template<typename TBase>
    uint8_t TDataModificationApprovalTransaction<TBase>::overlappingKeysCount() const
    {
        return overlappingKeysCount_;
    }

    template<typename TBase>
    uint8_t TDataModificationApprovalTransaction<TBase>::judgedKeysCount() const
    {
        return judgedKeysCount_;
    }

    template<typename TBase>
    uint8_t TDataModificationApprovalTransaction<TBase>::opinionElementCount() const
    {
        return opinionElementCount_;
    }

    template<typename TBase>
    std::vector<Key> TDataModificationApprovalTransaction<TBase>::publicKeys() const
    {
        return publicKeys_;
    }

    template<typename TBase>
    std::vector<Signature> TDataModificationApprovalTransaction<TBase>::signatures() const
    {
        return signatures_;
    }

    template<typename TBase>
    std::vector<uint8_t> TDataModificationApprovalTransaction<TBase>::presentOpinions() const
    {
        return presentOpinions_;
    }

    template<typename TBase>
    std::vector<uint64_t> TDataModificationApprovalTransaction<TBase>::opinions() const
    {
        return opinions_;
    }

	template class TDataModificationApprovalTransaction<Transaction>;
	template class TDataModificationApprovalTransaction<EmbeddedTransaction>;
}
