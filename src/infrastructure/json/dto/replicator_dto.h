/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <xpxchaincpp/model/storagev2/replicator_info.h>
#include <infrastructure/json/uint64.h>
#include <infrastructure/utils/variadic_struct.h>

#include <stdint.h>
#include <string>

namespace xpx_chain_sdk::internal::json::dto {

    using DriveInfoDto = VariadicStruct<
            Field<STR_LITERAL("drive"), std::string>,
            Field<STR_LITERAL("lastApprovedDataModificationId"), std::string>,
            Field<STR_LITERAL("dataModificationIdIsValid"), bool>,
            Field<STR_LITERAL("initialDownloadWork"), Uint64> >;
    
    using ReplicatorDto = VariadicStruct<
            Field<STR_LITERAL("replicatorKey"), std::string>,
            Field<STR_LITERAL("version"), Uint64>,
            Field<STR_LITERAL("capacity"), Uint64>,
            Field<STR_LITERAL("blskey"), std::string>,
            Field<STR_LITERAL("drives"), std::vector<DriveInfoDto> > >;
    
}