/**
*** Copyright 2023 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include <xpxchaincpp/model/transaction/unsuccessful_end_batch_execution_transaction.h>

namespace xpx_chain_sdk {
    template<typename TBase>
    const Key& TUnsuccessfulEndBatchExecutionTransaction<TBase>::contractKey() const {
        return contractKey_;
    }

    template<typename TBase>
    uint64_t TUnsuccessfulEndBatchExecutionTransaction<TBase>::batchId() const {
        return batchId_;
    }

    template<typename TBase>
    const uint64_t TUnsuccessfulEndBatchExecutionTransaction<TBase>::automaticExecutionsNextBlockToCheck() const {
        return automaticExecutionsNextBlockToCheck_;
    }

    template<typename TBase>
    const std::vector<ExtendedCallDigest>& TUnsuccessfulEndBatchExecutionTransaction<TBase>::callDigests() const {
        return callDigests_;
    }

    template<typename TBase>
    const std::vector<Opinion>& TUnsuccessfulEndBatchExecutionTransaction<TBase>::opinions() const {
        return opinions_;
    }

    template class TUnsuccessfulEndBatchExecutionTransaction<Transaction>;
    template class TUnsuccessfulEndBatchExecutionTransaction<EmbeddedTransaction>;
}