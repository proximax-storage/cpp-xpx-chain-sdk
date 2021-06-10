/*
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <string>
#include "infrastructure/utils/variadic_struct.h"
#include "infrastructure/json/uint32.h"
#include "infrastructure/json/uint64.h"

namespace xpx_chain_sdk::internal::json::dto {

    using TransactionDataDto = VariadicStruct<
        Field<STR_LITERAL("signature"), std::string>,
        Field<STR_LITERAL("signer"), std::string>,
        Field<STR_LITERAL("version"), Uint32>,
        Field<STR_LITERAL("type"), TransactionType>,
        Field<STR_LITERAL("maxFee"), Uint64>,
        Field<STR_LITERAL("deadline"), Uint64>,
        Field<STR_LITERAL("hash"), std::string>>;

    using TransactionMetaDto = VariadicStruct<
            Field<STR_LITERAL("hash"), std::string>,
            Field<STR_LITERAL("merkleComponentHash"), std::string>,
            Field<STR_LITERAL("height"), Uint64>>;

    using TransactionNotificationDto = VariadicStruct<
            Field<STR_LITERAL("transaction"), TransactionDataDto>,
            Field<STR_LITERAL("meta"), TransactionMetaDto>>;
}
