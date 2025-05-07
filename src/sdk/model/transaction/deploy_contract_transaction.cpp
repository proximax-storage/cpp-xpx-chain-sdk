/**
*** Copyright 2023 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include <xpxchaincpp/model/transaction/deploy_contract_transaction.h>

namespace xpx_chain_sdk {
    template<typename TBase>
    const xpx_chain_sdk::Key &xpx_chain_sdk::TDeployContractTransaction<TBase>::driveKey() const {
        return driveKey_;
    }

    template<typename TBase>
    const std::string &xpx_chain_sdk::TDeployContractTransaction<TBase>::fileName() const {
        return fileName_;
    }

    template<typename TBase>
    const std::string &xpx_chain_sdk::TDeployContractTransaction<TBase>::functionName() const {
        return functionName_;
    }

    template<typename TBase>
    const std::vector<uint8_t>& xpx_chain_sdk::TDeployContractTransaction<TBase>::actualArguments() const {
        return actualArguments_;
    }

    template<typename TBase>
    const Amount& xpx_chain_sdk::TDeployContractTransaction<TBase>::executionCallPayment() const {
        return executionCallPayment_;
    }

    template<typename TBase>
    const Amount& xpx_chain_sdk::TDeployContractTransaction<TBase>::downloadCallPayment() const {
        return downloadCallPayment_;
    }

    template<typename TBase>
    const MosaicContainer& xpx_chain_sdk::TDeployContractTransaction<TBase>::servicePayments() const {
        return servicePayments_;
    }

    template<typename TBase>
    const std::string &xpx_chain_sdk::TDeployContractTransaction<TBase>::automaticExecutionsFileName() const {
        return automaticExecutionsFileName_;
    }

    template<typename TBase>
    const std::string &xpx_chain_sdk::TDeployContractTransaction<TBase>::automaticExecutionsFunctionName() const {
        return automaticExecutionsFunctionName_;
    }

    template<typename TBase>
    const Amount & xpx_chain_sdk::TDeployContractTransaction<TBase>::automaticExecutionsCallPayment() const {
        return automaticExecutionsCallPayment_;
    }

    template<typename TBase>
    const Amount & xpx_chain_sdk::TDeployContractTransaction<TBase>::automaticDownloadCallPayment() const {
        return automaticDownloadCallPayment_;
    }

    template<typename TBase>
    uint32_t xpx_chain_sdk::TDeployContractTransaction<TBase>::automaticExecutionsNumber() const {
        return automaticExecutionsNumber_;
    }

    template<typename TBase>
    const xpx_chain_sdk::Key &xpx_chain_sdk::TDeployContractTransaction<TBase>::assignee() const {
        return assignee_;
    }

    template class TDeployContractTransaction<Transaction>;
    template class TDeployContractTransaction<EmbeddedTransaction>;
}