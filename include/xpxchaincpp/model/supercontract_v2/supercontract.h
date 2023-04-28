/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/types.h>

#include <map>
#include <set>
#include <deque>
#include <string>
#include <string_view>

namespace xpx_chain_sdk {

    struct AutomaticExecutionsInfo {
        std::string automaticExecutionFileName;
        std::string automaticExecutionsFunctionName;
        uint64_t automaticExecutionsNextBlockToCheck;
        uint64_t automaticExecutionCallPayment;
        uint64_t automaticDownloadCallPayment;
        uint32_t automatedExecutionsNumber = 0U;
        std::optional<uint64_t> automaticExecutionsPrepaidSince;
    };

    struct CompletedCall {
        std::string callId;
        std::string caller;
        int16_t status;
        uint64_t executionWork;
        uint64_t downloadWork;
    };

    struct Batch {
        bool success;
        std::array<uint8_t, 32> poExVerificationInformation;
        std::vector<CompletedCall> completedCalls;
    };

    struct ServicePayment {
        uint64_t mosaicId;
        uint64_t amount;
    };

    struct ContractCall {
        std::string callId;
        std::string caller;
        std::string fileName;
        std::string functionName;
        std::string actualArguments;
        uint64_t executionCallPayment;
        uint64_t downloadCallPayment;
        std::vector<ServicePayment> servicePayments;
        uint64_t blockHeight;
    };

    struct ProofOfExecution {
        uint64_t startBatchId = 0;
        std::array<uint8_t, 32> T;
        std::array<uint8_t, 32> R;
    };

    struct ExecutorInfo {
        uint64_t nextBatchToApprove = 0;
        ProofOfExecution poEx;
    };

    class SuperContractV2Data {
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

    class SuperContractV2 {
    public:
        SuperContractV2Data data;
    };

    class MultipleSuperContracts {
    public:
        std::vector<SuperContractV2> supercontracts;
    };
}
