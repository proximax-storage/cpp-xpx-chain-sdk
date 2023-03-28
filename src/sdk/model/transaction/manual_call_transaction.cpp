/**
*** Copyright 2023 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include <xpxchaincpp/model/transaction/manual_call_transaction.h>

namespace xpx_chain_sdk {
    template<typename TBase>
    const Key& TManualCallTransaction<TBase>::contractKey() const {
        return contractKey_;
    }

    template<typename TBase>
    const std::string& TManualCallTransaction<TBase>::fileName() const {
        return fileName_;
    }

    template<typename TBase>
    const std::string& TManualCallTransaction<TBase>::functionName() const {
        return functionName_;
    }

    template<typename TBase>
    const std::vector<uint8_t>& TManualCallTransaction<TBase>::actualArguments() const {
        return actualArguments_;
    }

    template<typename TBase>
    const Amount& TManualCallTransaction<TBase>::executionCallPayment() const {
        return executionCallPayment_;
    }

    template<typename TBase>
    const Amount& TManualCallTransaction<TBase>::downloadCallPayment() const {
        return downloadCallPayment_;
    }

    template<typename TBase>
    const std::vector<MosaicId>& TManualCallTransaction<TBase>::servicePayments() const {
        return servicePayments_;
    }

    template class TManualCallTransaction<Transaction>;
    template class TManualCallTransaction<EmbeddedTransaction>;
}
