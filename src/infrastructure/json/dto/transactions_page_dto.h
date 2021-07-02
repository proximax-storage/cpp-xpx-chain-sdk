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
#include "infrastructure/json/dto/transaction_dto.h"

namespace xpx_chain_sdk::internal::json::dto::transactions_page {

    using TransactionMetaDto = VariadicStruct<
            Field<STR_LITERAL("height"), Uint64>,
            Field<STR_LITERAL("hash"), std::string>,
            Field<STR_LITERAL("merkleComponentHash"), std::string>,
            Field<STR_LITERAL("index"), Uint32>>;

    using TransactionDto = VariadicStruct<
            Field<STR_LITERAL("meta"), TransactionMetaDto>,
            Field<STR_LITERAL("transaction"), TransactionDto>,
            Field<STR_LITERAL("id"), std::string>>;

    using PaginationDto = VariadicStruct<
            Field<STR_LITERAL("totalEntries"), uint64_t>,
            Field<STR_LITERAL("pageNumber"), uint64_t>,
            Field<STR_LITERAL("pageSize"), uint64_t>,
            Field<STR_LITERAL("totalPages"), uint64_t>>;

    using TransactionsPageDto = VariadicStruct<
            Field<STR_LITERAL("data"), std::vector<TransactionDto>>,
            Field<STR_LITERAL("pagination"), PaginationDto>>;
}
