
/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/
#pragma once

#include <infrastructure/utils/variadic_struct.h>
#include <infrastructure/json/uint64.h>

#include <map>
#include <set>
#include <deque>
#include <string>
#include <vector>
#include <optional>

namespace xpx_chain_sdk::internal::json::dto {
    using internal::json::Uint64;

    using AutomaticExecutionsInfoDto = VariadicStruct<
        Field<STR_LITERAL("automaticExecutionFileName"), std::string >,
        Field<STR_LITERAL("automaticExecutionsFunctionName"), std::string >,
        Field<STR_LITERAL("automaticExecutionsNextBlockToCheck"), Uint64 >,
        Field<STR_LITERAL("automaticExecutionCallPayment"), Uint64 >,
        Field<STR_LITERAL("automaticDownloadCallPayment"), Uint64 >,
        Field<STR_LITERAL("automatedExecutionsNumber"), uint32_t >,
        Field<STR_LITERAL("automaticExecutionsPrepaidSince"), Uint64 >
    >;

    using ServicePaymentDto = VariadicStruct<
        Field<STR_LITERAL("mosaicId"), Uint64 >,
        Field<STR_LITERAL("amount"), Uint64 >
    >;

    using ContractCallDto = VariadicStruct<
        Field<STR_LITERAL("callId"), std::string >,
        Field<STR_LITERAL("caller"), std::string >,
        Field<STR_LITERAL("fileName"), std::string >,
        Field<STR_LITERAL("functionName"), std::string >,
        Field<STR_LITERAL("actualArguments"), std::string >,
        Field<STR_LITERAL("executionCallPayment"), Uint64 >,
        Field<STR_LITERAL("downloadCallPayment"), Uint64 >,
        Field<STR_LITERAL("servicePayments"), std::vector<ServicePaymentDto> >,
        Field<STR_LITERAL("blockHeight"), Uint64 >
    >;

    using CompletedCallDto = VariadicStruct<
        Field<STR_LITERAL("callId"), std::string >,
        Field<STR_LITERAL("caller"), std::string >,
        Field<STR_LITERAL("status"), int16_t >,
        Field<STR_LITERAL("executionWork"), Uint64 >,
        Field<STR_LITERAL("downloadWork"), Uint64 >
    >;

    using ProofOfExecutionDto = VariadicStruct<
        Field<STR_LITERAL("startBatchId"), Uint64 >,
        Field<STR_LITERAL("T"), std::array<uint8_t, 32> >,
        Field<STR_LITERAL("R"), std::array<uint8_t, 32> >
    >;

    using ExecutorInfoDto = VariadicStruct<
        Field<STR_LITERAL("replicatorKey"), std::string >,
        Field<STR_LITERAL("nextBatchToApprove"), Uint64 >,
        Field<STR_LITERAL("poEx"), ProofOfExecutionDto >
    >;

    using BatchDto = VariadicStruct<
        Field<STR_LITERAL("batchId"), Uint64 >,
        Field<STR_LITERAL("success"), bool >,
        Field<STR_LITERAL("poExVerificationInformation"), std::array<uint8_t, 32> >,
        Field<STR_LITERAL("completedCalls"), std::vector<CompletedCallDto> >
    >;

    using SuperContractV2Dto = VariadicStruct<
        Field<STR_LITERAL("supercontract"),
            VariadicStruct<
                Field<STR_LITERAL("contractKey"), std::string >,
                Field<STR_LITERAL("executionPaymentKey"), std::string >,
                Field<STR_LITERAL("assignee"), std::string >,
                Field<STR_LITERAL("creator"), std::string >,
                Field<STR_LITERAL("deploymentBaseModificationId"), std::string >,
                Field<STR_LITERAL("automaticExecutionsInfo"), AutomaticExecutionsInfoDto >,
                Field<STR_LITERAL("requestedCalls"), std::vector<ContractCallDto> >,
                Field<STR_LITERAL("executorsInfo"), std::vector<ExecutorInfoDto> >,
                Field<STR_LITERAL("batches"), std::vector<BatchDto> >,
                Field<STR_LITERAL("releasedTransactions"), std::vector<std::string> >
            >
        >
    >;
}

