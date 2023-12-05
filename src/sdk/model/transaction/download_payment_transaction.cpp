/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/download_payment_transaction.h>

namespace xpx_chain_sdk {
	
	template<typename TBase>
	const Hash256& TDownloadPaymentTransaction<TBase>::downloadChannelId() const
	{
		return downloadChannelId_;
	}

	template<typename TBase>
	uint64_t TDownloadPaymentTransaction<TBase>::downloadSize() const
	{
		return downloadSize_;
	}

	template<typename TBase>
	const Amount& TDownloadPaymentTransaction<TBase>::feedbackFeeAmount() const
	{
		return feedbackFeeAmount_;
	}

	template class TDownloadPaymentTransaction<Transaction>;
	template class TDownloadPaymentTransaction<EmbeddedTransaction>;
}
