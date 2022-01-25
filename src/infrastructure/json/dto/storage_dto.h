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
            Field<STR_LITERAL("downloadChannelId"), std::string>,
            Field<STR_LITERAL("replicators"), std::vector<std::string> > >;

    using VerificationDto = VariadicStruct<
            Field<STR_LITERAL("verificationTrigger"), std::string>,
            Field<STR_LITERAL("expiration"), Uint64>,
            Field<STR_LITERAL("expired"), bool>,
            Field<STR_LITERAL("shards"), std::vector<ShardDto> >>;

    using DriveDataDto = VariadicStruct<
            Field<STR_LITERAL("multisig"), std::string>,
            Field<STR_LITERAL("multisigAddress"), std::string>,
            Field<STR_LITERAL("owner"), std::string>,
            Field<STR_LITERAL("rootHash"), std::string>,
            Field<STR_LITERAL("size"), Uint64 >,
            Field<STR_LITERAL("usedSize"), Uint64 >,
            Field<STR_LITERAL("metaFilesSize"), Uint64 >,
            Field<STR_LITERAL("replicatorCount"), Uint32 >,
            Field<STR_LITERAL("ownerCumulativeUploadSize"), Uint64 >,
            Field<STR_LITERAL("activeDataModifications"), std::vector<ActiveDataModificationDto> >,
            Field<STR_LITERAL("completedDataModifications"), std::vector<CompletedDataModificationDto> >,
            Field<STR_LITERAL("confirmedUsedSizes"), std::vector<ConfirmedUsedSizeDto> >,
            Field<STR_LITERAL("replicators"), std::vector<std::string> >,
            Field<STR_LITERAL("offboardingReplicators"), std::vector<std::string> >,
            Field<STR_LITERAL("verifications"), std::vector<VerificationDto> > >;

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

    using MultipleReplicatorsDto = std::vector<ReplicatorDto>;

    using CumulativePaymentDto = VariadicStruct<
            Field<STR_LITERAL("replicator"), std::string>,
            Field<STR_LITERAL("payment"), uint64_t>>;

    using DownloadChannelDataDto = VariadicStruct<
            Field<STR_LITERAL("id"), std::string>,
            Field<STR_LITERAL("consumer"), std::string>,
            Field<STR_LITERAL("drive"), std::string>,
            Field<STR_LITERAL("downloadSize"), Uint64>,
            Field<STR_LITERAL("downloadApprovalCount"), uint16_t>,
            Field<STR_LITERAL("listOfPublicKeys"), std::vector<std::string>>,
            Field<STR_LITERAL("cumulativePayments"), std::vector<CumulativePaymentDto> >>;

    using DownloadChannelDto = VariadicStruct<
            Field<STR_LITERAL("downloadChannelInfo"), DownloadChannelDataDto > >;

    using MultipleDownloadChannelsDto = std::vector<DownloadChannelDto>;
}
