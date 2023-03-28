/**
*** Copyright 2023 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include <xpxchaincpp/model/transaction/automatic_execution_transaction.h>

namespace xpx_chain_sdk {

    template<typename TBase>
    const Key& TAutomaticExecutionsPaymentTransaction<TBase>::contractKey() const {
        return contractKey_;
    }


    template<typename TBase>
    uint32_t TAutomaticExecutionsPaymentTransaction<TBase>::automaticExecutionsNumber() const {
        return automaticExecutionsNumber_;
    }

    template class TAutomaticExecutionsPaymentTransaction<Transaction>;
    template class TAutomaticExecutionsPaymentTransaction<EmbeddedTransaction>;
}