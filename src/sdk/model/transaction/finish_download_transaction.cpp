/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/finish_download_transaction.h>

namespace xpx_chain_sdk {
	
	template<typename TBase>
	const Hash256& TFinishDownloadTransaction<TBase>::downloadChannelId() const
	{
		return downloadChannelId_;
	}

	template<typename TBase>
	const Amount& TFinishDownloadTransaction<TBase>::feedbackFeeAmount() const
	{
		return feedbackFeeAmount_;
	}

	template class TFinishDownloadTransaction<Transaction>;
	template class TFinishDownloadTransaction<EmbeddedTransaction>;
}
