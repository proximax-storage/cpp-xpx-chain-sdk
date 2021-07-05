/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <string>
#include <stdint.h>
#include <vector>
#include <infrastructure/utils/variadic_struct.h>
#include <infrastructure/json/uint64.h>


namespace xpx_chain_sdk::internal::json::dto {
    using NetworkConfigDataDto = VariadicStruct<
            Field<STR_LITERAL("height"), Uint64>,
            Field<STR_LITERAL("networkConfig"), std::string>,
            Field<STR_LITERAL("supportedEntityVersions"), std::string> >;

    using NetworkConfigDto = VariadicStruct<
            Field<STR_LITERAL("networkConfig"), NetworkConfigDataDto> >;
}