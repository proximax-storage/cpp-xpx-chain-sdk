/**
*** Copyright 2023 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/transaction/stream_payment_transaction.h>

namespace xpx_chain_sdk {

    template<typename TBase>
    const Key& TStreamPaymentTransaction<TBase>::driveKey() const {
        return driveKey_;
    }

    template<typename TBase>
    const Hash256& TStreamPaymentTransaction<TBase>::streamId() const {
        return streamId_;
    }

    template<typename TBase>
    uint64_t TStreamPaymentTransaction<TBase>::additionalUploadSizeMegabytes() const {
        return additionalUploadSizeMegabytes_;
    }

    template class TStreamPaymentTransaction<Transaction>;
    template class TStreamPaymentTransaction<EmbeddedTransaction>;
}
