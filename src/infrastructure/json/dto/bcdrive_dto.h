/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <xpxchaincpp/model/storagev2/bcdrive_info.h>
#include <infrastructure/json/uint64.h>
#include <infrastructure/utils/variadic_struct.h>
#include "infrastructure/json/uint64.h"

#include <stdint.h>
#include <string>

namespace xpx_chain_sdk::internal::json::dto {

    using ActiveDataModificationDto = VariadicStruct<
            Field<STR_LITERAL("id"), std::string>,
            Field<STR_LITERAL("owner"), std::string>,
            Field<STR_LITERAL("downloadDataCdi"), std::string>,
            Field<STR_LITERAL("expectedUploadSize"), Uint64>, 
            Field<STR_LITERAL("actualUploadSize"), Uint64>,
            Field<STR_LITERAL("folderName"), std::string>, 
            Field<STR_LITERAL("readyForApproval"), bool> >;

    using CompletedDataModificationDto = VariadicStruct<
            Field<STR_LITERAL("activeDataModifications"), std::vector<ActiveDataModificationDto> >,
            Field<STR_LITERAL("state"), DataModificationState> >;

    using ConfirmedUsedSizeDTO = VariadicStruct<
            Field<STR_LITERAL("id"), std::string>,
            Field<STR_LITERAL("size"), Uint64> >; 
    
    using VerificationOpinionDTO = VariadicStruct<
            Field<STR_LITERAL("prover"), std::string>,
            Field<STR_LITERAL("result"), uint8_t> >; 
    
    using VerificationDTO = VariadicStruct<
            Field<STR_LITERAL("verificationTrigger"), std::string>,
            Field<STR_LITERAL("state"), VerificationState>, 
            Field<STR_LITERAL("verificationOpinions"), std::vector<VerificationOpinionDTO> > >; 

    //vector
    using ActiveDataModificationsDtos = std::vector<ActiveDataModificationDto>;
    using CompletedDataModificationsDtos = std::vector<CompletedDataModificationDto>;
    using ComfirmedUsedSizesDtos = std::vector<ConfirmedUsedSizeDTO>;
    using ReplicatorsListDtos = std::vector<std::string>;
    using VerificationsDtos = std::vector<VerificationDTO>;

    
    using BcDriveDto = VariadicStruct<
            Field<STR_LITERAL("driveKey"), std::string>,
            Field<STR_LITERAL("owner"), std::string>,
            Field<STR_LITERAL("rootHash"), std::string>,
            Field<STR_LITERAL("size"), Uint64>,
            Field<STR_LITERAL("usedSize"), Uint64>,
            Field<STR_LITERAL("metaFilesSize"), Uint64>,
            Field<STR_LITERAL("replicatorCount"), Uint64>,
            Field<STR_LITERAL("activeDataModifications"), ActiveDataModificationsDtos >,
            Field<STR_LITERAL("completedDataModifications"), CompletedDataModificationsDtos >,
            Field<STR_LITERAL("confirmedUsedSizes"), ComfirmedUsedSizesDtos >,
            Field<STR_LITERAL("replicators"), ReplicatorsListDtos >,
            Field<STR_LITERAL("verifications"), VerificationsDtos > >;
            
}