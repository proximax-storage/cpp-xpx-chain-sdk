/**
*** Copyright 2023 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/stream_start_transaction.h>

namespace xpx_chain_sdk {

    template<typename TBase>
    const Key& TStreamStartTransaction<TBase>::driveKey() const {
        return driveKey_;
    }

    template<typename TBase>
    uint64_t TStreamStartTransaction<TBase>::expectedUploadSizeMegabytes() const {
        return expectedUploadSizeMegabytes_;
    }

    template<typename TBase>
    std::vector<uint8_t> TStreamStartTransaction<TBase>::folderName() const {
        return folderName_;
    }

    template<typename TBase>
    uint16_t TStreamStartTransaction<TBase>::folderNameSize() const {
        return folderNameSize_;
    }

    template<typename TBase>
    const Amount& TStreamStartTransaction<TBase>::feedbackFeeAmount() const {
        return feedbackFeeAmount_;
    }

    template class TStreamStartTransaction<Transaction>;
    template class TStreamStartTransaction<EmbeddedTransaction>;
}
