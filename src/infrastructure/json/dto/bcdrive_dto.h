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
            Field<STR_LITERAL("uploadSize"), Uint64> >;
    
    using CompletedDataModificationDto = VariadicStruct<
            Field<STR_LITERAL("activeDataModification"), std::vector<ActiveDataModificationDto> >,
            Field<STR_LITERAL("state"), DataModificationState> >;

    using BcDriveDto = VariadicStruct<
            Field<STR_LITERAL("driveKey"), std::string>,
            Field<STR_LITERAL("owner"), std::string>,
            Field<STR_LITERAL("rootHash"), std::string>,
            Field<STR_LITERAL("size"), Uint64>,
            Field<STR_LITERAL("usedSize"), Uint64>,
            Field<STR_LITERAL("metaFilesSize"), Uint64>,
            Field<STR_LITERAL("replicatorCount"), Uint64>,
            Field<STR_LITERAL("activeDataModifications"), std::vector<ActiveDataModificationDto> >,
            Field<STR_LITERAL("completedDataModifications"), std::vector<CompletedDataModificationDto> > >;
}