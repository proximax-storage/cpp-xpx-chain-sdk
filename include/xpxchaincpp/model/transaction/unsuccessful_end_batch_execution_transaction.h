/**
*** Copyright 2023 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/model/transaction/transaction.h>
#include <xpxchaincpp/model/transaction/embedded_transaction.h>
#include <xpxchaincpp/model/transaction/end_batch_execution_transaction_types.h>

#include <optional>

namespace xpx_chain_sdk {
	
	/// Unsuccessful end batch execution transaction base class.
	template<typename TBase>
    class TUnsuccessfulEndBatchExecutionTransaction: public TBase {
	public:
		/// Creates Unsuccessful end batch execution transaction.
		template<typename... TArgs>
		explicit TUnsuccessfulEndBatchExecutionTransaction(
                const Key& contractKey,
				uint64_t batchId,
				const uint64_t automaticExecutionsNextBlockToCheck,
				const std::vector<ExtendedCallDigest>& callDigests,
				const std::vector<Opinion>& opinions,
		        TArgs&&... args):
            TBase(TransactionType::Automatic_Executions_Payment, std::forward<TArgs>(args)...),
            contractKey_(contractKey),
			batchId_(batchId),
			automaticExecutionsNextBlockToCheck_(automaticExecutionsNextBlockToCheck),
			callDigests_(callDigests),
			opinions_(opinions)
		{ }

        const Key& contractKey() const;

        uint64_t batchId() const;

        const uint64_t automaticExecutionsNextBlockToCheck() const;

        const std::vector<ExtendedCallDigest>& callDigests() const;

        const std::vector<Opinion>& opinions() const;

	private:
        Key contractKey_;
		uint64_t batchId_;
		uint64_t automaticExecutionsNextBlockToCheck_;
		std::vector<ExtendedCallDigest> callDigests_;
		std::vector<Opinion> opinions_;
	};

	extern template class TUnsuccessfulEndBatchExecutionTransaction<Transaction>;
	extern template class TUnsuccessfulEndBatchExecutionTransaction<EmbeddedTransaction>;

	using UnsuccessfulEndBatchExecutionTransaction = TUnsuccessfulEndBatchExecutionTransaction<Transaction>;
	using EmbeddedUnsuccessfulEndBatchExecutionTransaction = TUnsuccessfulEndBatchExecutionTransaction<EmbeddedTransaction>;
}
