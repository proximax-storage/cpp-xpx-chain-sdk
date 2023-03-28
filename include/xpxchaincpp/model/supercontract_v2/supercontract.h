/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include "automatic_executions_info.h"
#include "batch.h"
#include "contract_call.h"
#include "executor_info.h"
#include "executor_info.h"
#include <xpxchaincpp/types.h>

#include <map>
#include <set>
#include <deque>
#include <string>
#include <string_view>

namespace xpx_chain_sdk {

    /// SuperContract.
    class SuperContract {
    public:
        const Key& driveKey() const;

        const Key& executionPaymentKey() const;

        const Key& assignee() const;

        const Key& creator() const;

        const Hash256& deploymentBaseModificationId() const;

        const AutomaticExecutionsInfo& automaticExecutionsInfo() const;

        const std::deque<ContractCall>& requestedCalls() const;

        const std::map<Key, ExecutorInfo>& executorsInfo() const;

        const std::map<uint64_t, Batch>& batches() const;

        const std::multiset<Hash256>& releasedTransactions() const;

    private:
        Key driveKey_;
        Key executionPaymentKey_;
        Key assignee_;
        Key creator_;
        Hash256 deploymentBaseModificationId_;
        AutomaticExecutionsInfo automaticExecutionsInfo_;
        std::deque<ContractCall> requestedCalls_;
        std::map<Key, ExecutorInfo> executorsInfo_;
        std::map<uint64_t, Batch> batches_;
        std::multiset<Hash256> releasedTransactions_;
    };

    /// Returns \c true if \a lhs and \a rhs are equal.
    bool operator==(const SuperContract& lhs, const SuperContract& rhs);

    /// Returns \c true if \a lhs and \a rhs are not equal.
    bool operator!=(const SuperContract& lhs, const SuperContract& rhs);
}
