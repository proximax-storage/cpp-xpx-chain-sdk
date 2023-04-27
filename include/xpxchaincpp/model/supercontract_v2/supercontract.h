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
#include <xpxchaincpp/types.h>

#include <map>
#include <set>
#include <deque>
#include <string>
#include <string_view>

namespace xpx_chain_sdk {

    /// SuperContractInfo.
    class SuperContractInfo {
    public:
        std::string contractKey;
        std::string executionPaymentKey;
        std::string assignee;
        std::string creator;
        std::string deploymentBaseModificationId;
        AutomaticExecutionsInfo automaticExecutionsInfo;
        std::deque<ContractCall> requestedCalls;
        std::map<std::string, ExecutorInfo> executorsInfo;
        std::map<uint64_t, Batch> batches;
        std::multiset<std::string> releasedTransactions;
    };
}
