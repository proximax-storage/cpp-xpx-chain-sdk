/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/download_transaction.h>

namespace xpx_chain_sdk {
	
	template<typename TBase>
	const Key& TDownloadTransaction<TBase>::driveKey() const
	{
		return driveKey_;
	}

	template<typename TBase>
	uint64_t TDownloadTransaction<TBase>::downloadSize() const
	{
		return downloadSize_;
	}

	template<typename TBase>
	const Amount& TDownloadTransaction<TBase>::feedbackFeeAmount() const
	{
		return feedbackFeeAmount_;
	}

    template<typename TBase>
    uint16_t TDownloadTransaction<TBase>::listOfPublicKeysSize() const
    {
        return listOfPublicKeysSize_;
    }

	template<typename TBase>
    std::vector<Key> TDownloadTransaction<TBase>::listOfPublicKeys() const
	{
		return listOfPublicKeys_;
	}

	template class TDownloadTransaction<Transaction>;
	template class TDownloadTransaction<EmbeddedTransaction>;
}
