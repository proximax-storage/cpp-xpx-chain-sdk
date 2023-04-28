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

	/// Successful rnd batch execution transaction base class.
	template<typename TBase>
    class TSuccessfulEndBatchExecutionTransaction: public TBase {
	public:
		/// Creates Successful rnd batch execution transaction.
		template<typename... TArgs>
		explicit TSuccessfulEndBatchExecutionTransaction(
                const Key& contractKey,
				uint64_t batchId,
				const Hash256& storageHash,
				uint64_t usedSizeBytes,
				uint64_t metaFilesSizeBytes,
				const uint64_t automaticExecutionsNextBlockToCheck,
				const std::array<uint8_t, 32>& proofOfExecutionVerificationInformation,
				const std::vector<ExtendedCallDigest>& callDigests,
				const std::vector<Opinion>& opinions,
		        TArgs&&... args):
            TBase(TransactionType::Automatic_Executions_Payment, std::forward<TArgs>(args)...),
            contractKey_(contractKey),
			batchId_(batchId),
			storageHash_(storageHash),
			usedSizeBytes_(usedSizeBytes),
			metaFilesSizeBytes_(metaFilesSizeBytes),
			automaticExecutionsNextBlockToCheck_(automaticExecutionsNextBlockToCheck),
			proofOfExecutionVerificationInformation_(proofOfExecutionVerificationInformation),
			callDigests_(callDigests),
			opinions_(opinions)
		{ }

        const Key& contractKey() const;

        uint64_t batchId() const;

        const Hash256& storageHash() const;

        uint64_t usedSizeBytes() const;

        uint64_t metaFilesSizeBytes() const;

        const uint64_t automaticExecutionsNextBlockToCheck() const;

        const std::array<uint8_t, 32>& proofOfExecutionVerificationInformation() const;

        const std::vector<ExtendedCallDigest>& callDigests() const;

        const std::vector<Opinion>& opinions() const;


    private:
        Key contractKey_;
		uint64_t batchId_;
		Hash256 storageHash_;
		uint64_t usedSizeBytes_;
		uint64_t metaFilesSizeBytes_;
		uint64_t automaticExecutionsNextBlockToCheck_;
		std::array<uint8_t, 32> proofOfExecutionVerificationInformation_;
		std::vector<ExtendedCallDigest> callDigests_;
		std::vector<Opinion> opinions_;
	};

	extern template class TSuccessfulEndBatchExecutionTransaction<Transaction>;
	extern template class TSuccessfulEndBatchExecutionTransaction<EmbeddedTransaction>;

	using SuccessfulEndBatchExecutionTransaction = TSuccessfulEndBatchExecutionTransaction<Transaction>;
	using EmbeddedSuccessfulEndBatchExecutionTransaction = TSuccessfulEndBatchExecutionTransaction<EmbeddedTransaction>;
}
