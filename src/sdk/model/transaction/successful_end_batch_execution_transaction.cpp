/**
*** Copyright 2023 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include <xpxchaincpp/model/transaction/successful_end_batch_execution_transaction.h>

namespace xpx_chain_sdk {
    template<typename TBase>
    const Key& TSuccessfulEndBatchExecutionTransaction<TBase>::contractKey() const {
        return contractKey_;
    }

    template<typename TBase>
    uint64_t TSuccessfulEndBatchExecutionTransaction<TBase>::batchId() const {
        return batchId_;
    }

    template<typename TBase>
    const Hash256& TSuccessfulEndBatchExecutionTransaction<TBase>::storageHash() const {
        return storageHash_;
    }

    template<typename TBase>
    uint64_t TSuccessfulEndBatchExecutionTransaction<TBase>::usedSizeBytes() const {
        return usedSizeBytes_;
    }

    template<typename TBase>
    uint64_t TSuccessfulEndBatchExecutionTransaction<TBase>::metaFilesSizeBytes() const {
        return metaFilesSizeBytes_;
    }

    template<typename TBase>
    const Height& TSuccessfulEndBatchExecutionTransaction<TBase>::automaticExecutionsNextBlockToCheck() const {
        return automaticExecutionsNextBlockToCheck_;
    }

    template<typename TBase>
    const std::array<uint8_t, 32>& TSuccessfulEndBatchExecutionTransaction<TBase>::proofOfExecutionVerificationInformation() const {
        return proofOfExecutionVerificationInformation_;
    }

    template<typename TBase>
    const std::vector<ExtendedCallDigest>& TSuccessfulEndBatchExecutionTransaction<TBase>::callDigests() const {
        return callDigests_;
    }

    template<typename TBase>
    const std::vector<Opinion>& TSuccessfulEndBatchExecutionTransaction<TBase>::opinions() const {
        return opinions_;
    }
    template class TSuccessfulEndBatchExecutionTransaction<Transaction>;
    template class TSuccessfulEndBatchExecutionTransaction<EmbeddedTransaction>;
}