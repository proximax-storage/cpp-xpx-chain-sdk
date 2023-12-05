/*
*** Copyright 2022 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <string>
#include "infrastructure/utils/variadic_struct.h"
#include "infrastructure/json/uint32.h"
#include "infrastructure/json/uint64.h"
#include "infrastructure/json/dto/storage_dto.h"

namespace xpx_chain_sdk::internal::json::dto::replicators_page {

    using PaginationDto = VariadicStruct<
            Field<STR_LITERAL("totalEntries"), uint64_t>,
            Field<STR_LITERAL("pageNumber"), uint64_t>,
            Field<STR_LITERAL("pageSize"), uint64_t>,
            Field<STR_LITERAL("totalPages"), uint64_t>>;

    using ReplicatorsPageDto = VariadicStruct<
            Field<STR_LITERAL("data"), std::vector<ReplicatorDto>>,
            Field<STR_LITERAL("pagination"), PaginationDto>>;
}
