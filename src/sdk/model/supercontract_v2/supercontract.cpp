/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/model/supercontract_v2/supercontract.h>

namespace xpx_chain_sdk {
    const Key& SuperContract::driveKey() const {
        return driveKey_;
    }

    const Key& SuperContract::executionPaymentKey() const {
        return executionPaymentKey_;
    }

    const Key& SuperContract::assignee() const {
        return assignee_;
    }

    const Key& SuperContract::creator() const {
        return creator_;
    }

    const Hash256& SuperContract::deploymentBaseModificationId() const {
        return deploymentBaseModificationId_;
    }

    const AutomaticExecutionsInfo& SuperContract::automaticExecutionsInfo() const {
        return automaticExecutionsInfo_;
    }

    const std::deque<ContractCall>& SuperContract::requestedCalls() const {
        return requestedCalls_;
    }

    const std::map<Key, ExecutorInfo>& SuperContract::executorsInfo() const {
        return executorsInfo_;
    }

    const std::map<uint64_t, Batch>& SuperContract::batches() const {
        return batches_;
    }

    const std::multiset<Hash256>& SuperContract::releasedTransactions() const {
        return releasedTransactions_;
    }

    bool operator==(const SuperContract& lhs, const SuperContract& rhs)
    {
        return lhs.driveKey() == rhs.driveKey() &&
        lhs.executorsInfo() == rhs.executorsInfo() &&
        lhs.assignee() == rhs.assignee() &&
        lhs.creator() == rhs.creator() &&
        lhs.deploymentBaseModificationId() == rhs.deploymentBaseModificationId() &&
        lhs.automaticExecutionsInfo() == rhs.automaticExecutionsInfo() &&
        lhs.requestedCalls() == rhs.requestedCalls() &&
        lhs.executorsInfo() == rhs.executorsInfo() &&
        lhs.batches() == rhs.batches() &&
        lhs.releasedTransactions() == rhs.releasedTransactions();
    }

    bool operator!=(const SuperContract& lhs, const SuperContract& rhs)
    {
        return !(lhs == rhs);
    }
}