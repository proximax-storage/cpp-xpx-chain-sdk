/*
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include "infrastructure/utils/variadic_struct.h"
#include "infrastructure/json/uint64.h"
#include "infrastructure/json/descriptors.h"
#include <cstdint>

namespace xpx_chain_sdk::internal::json::dto {

	using Uint64 = xpx_chain_sdk::internal::json::Uint64;
	using Uint32 = xpx_chain_sdk::internal::json::Uint32;

    using ActiveDataModificationDto = VariadicStruct<
            Field<STR_LITERAL("id"), std::string>,
            Field<STR_LITERAL("owner"), std::string>,
            Field<STR_LITERAL("downloadDataCdi"), std::string>,
            Field<STR_LITERAL("expectedUploadSize"), Uint64 >,
            Field<STR_LITERAL("actualUploadSize"), Uint64 >,
            Field<STR_LITERAL("folderName"), std::string >,
            Field<STR_LITERAL("readyForApproval"), bool > >;

    using CompletedDataModificationDto = VariadicStruct<
            Field<STR_LITERAL("id"), std::string>,
            Field<STR_LITERAL("owner"), std::string>,
            Field<STR_LITERAL("downloadDataCdi"), std::string>,
            Field<STR_LITERAL("expectedUploadSize"), Uint64 >,
            Field<STR_LITERAL("actualUploadSize"), Uint64 >,
            Field<STR_LITERAL("folderName"), std::string >,
            Field<STR_LITERAL("readyForApproval"), bool >,
            Field<STR_LITERAL("state"), uint8_t > >;

    using ConfirmedUsedSizeDto = VariadicStruct<
            Field<STR_LITERAL("replicator"), std::string>,
            Field<STR_LITERAL("size"), Uint64> >;

    using ShardDto = VariadicStruct<
            Field<STR_LITERAL("id"), Uint32>,
            Field<STR_LITERAL("replicators"), std::vector<std::string> > >;

    using VerificationDto = VariadicStruct<
            Field<STR_LITERAL("verificationTrigger"), std::string>,
            Field<STR_LITERAL("expiration"), Uint64>,
            Field<STR_LITERAL("duration"), Uint32>,
            Field<STR_LITERAL("shards"), std::vector<ShardDto> >>;

    using DownloadShardDto = VariadicStruct<
            Field<STR_LITERAL("downloadChannelId"), std::string> >;

    using UploadInfoDto = VariadicStruct<
            Field<STR_LITERAL("key"), std::string>,
            Field<STR_LITERAL("uploadSize"), Uint64>>;

    using DataModificationShardDto = VariadicStruct<
            Field<STR_LITERAL("replicator"), std::string>,
            Field<STR_LITERAL("actualShardReplicators"), std::vector<UploadInfoDto>>,
            Field<STR_LITERAL("formerShardReplicators"), std::vector<UploadInfoDto>>,
            Field<STR_LITERAL("ownerUpload"), Uint64>>;

    using DriveDataDto = VariadicStruct<
            Field<STR_LITERAL("multisig"), std::string>,
            Field<STR_LITERAL("multisigAddress"), std::string>,
            Field<STR_LITERAL("owner"), std::string>,
            Field<STR_LITERAL("rootHash"), std::string>,
            Field<STR_LITERAL("size"), Uint64 >,
            Field<STR_LITERAL("usedSizeBytes"), Uint64 >,
            Field<STR_LITERAL("metaFilesSizeBytes"), Uint64 >,
            Field<STR_LITERAL("replicatorCount"), uint16_t >,
            Field<STR_LITERAL("activeDataModifications"), std::vector<ActiveDataModificationDto> >,
            Field<STR_LITERAL("completedDataModifications"), std::vector<CompletedDataModificationDto> >,
            Field<STR_LITERAL("confirmedUsedSizes"), std::vector<ConfirmedUsedSizeDto> >,
            Field<STR_LITERAL("replicators"), std::vector<std::string> >,
            Field<STR_LITERAL("offboardingReplicators"), std::vector<std::string> >,
            Field<STR_LITERAL("verification"), VerificationDto >,
            Field<STR_LITERAL("downloadShards"), std::vector<DownloadShardDto> >,
            Field<STR_LITERAL("dataModificationShards"), std::vector<DataModificationShardDto> > >;

    using DriveDto = VariadicStruct<
            Field<STR_LITERAL("drive"), DriveDataDto > >;

	using MultipleDrivesDto = std::vector<DriveDto>;

    using DriveInfoDto = VariadicStruct<
            Field<STR_LITERAL("drive"), std::string>,
            Field<STR_LITERAL("lastApprovedDataModificationId"), std::string>,
            Field<STR_LITERAL("dataModificationIdIsValid"), uint8_t>,
            Field<STR_LITERAL("initialDownloadWork"), Uint64>,
            Field<STR_LITERAL("lastCompletedCumulativeDownloadWork"), Uint64>>;

    using ReplicatorDataDto = VariadicStruct<
            Field<STR_LITERAL("key"), std::string>,
            Field<STR_LITERAL("version"), Uint32>,
            Field<STR_LITERAL("capacity"), Uint64>,
            Field<STR_LITERAL("drives"), std::vector<DriveInfoDto> >>;

    using ReplicatorDto = VariadicStruct<
            Field<STR_LITERAL("replicator"), ReplicatorDataDto > >;

    using CumulativePaymentDto = VariadicStruct<
            Field<STR_LITERAL("replicator"), std::string>,
            Field<STR_LITERAL("payment"), Uint64>>;

    using DownloadChannelDataDto = VariadicStruct<
            Field<STR_LITERAL("id"), std::string>,
            Field<STR_LITERAL("consumer"), std::string>,
            Field<STR_LITERAL("drive"), std::string>,
            Field<STR_LITERAL("downloadSizeMegabytes"), Uint64>,
            Field<STR_LITERAL("downloadApprovalCount"), uint16_t>,
            Field<STR_LITERAL("finished"), bool>,
            Field<STR_LITERAL("listOfPublicKeys"), std::vector<std::string>>,
            Field<STR_LITERAL("shardReplicators"), std::vector<std::string>>,
            Field<STR_LITERAL("cumulativePayments"), std::vector<CumulativePaymentDto> >>;

    using DownloadChannelDto = VariadicStruct<
            Field<STR_LITERAL("downloadChannelInfo"), DownloadChannelDataDto > >;
}
