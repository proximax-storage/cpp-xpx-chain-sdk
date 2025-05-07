/**
*** Copyright 2023 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/stream_finish_transaction.h>

namespace xpx_chain_sdk {

    template<typename TBase>
    const Key& TStreamFinishTransaction<TBase>::driveKey() const {
        return driveKey_;
    }

    template<typename TBase>
    const Hash256& TStreamFinishTransaction<TBase>::streamId() const {
        return streamId_;
    }

    template<typename TBase>
    uint64_t TStreamFinishTransaction<TBase>::actualUploadSizeMegabytes() const {
        return actualUploadSizeMegabytes_;
    }

    template<typename TBase>
    const Hash256& TStreamFinishTransaction<TBase>::streamStructureCdi() const {
        return streamStructureCdi_;
    }

    template class TStreamFinishTransaction<Transaction>;
    template class TStreamFinishTransaction<EmbeddedTransaction>;
}
