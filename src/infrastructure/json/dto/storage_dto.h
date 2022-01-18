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

    using DriveDataDto = VariadicStruct<
            Field<STR_LITERAL("multisig"), std::string>,
            Field<STR_LITERAL("owner"), std::string>,
            Field<STR_LITERAL("rootHash"), std::string>,
            Field<STR_LITERAL("size"), Uint64 >,
            Field<STR_LITERAL("usedSize"), Uint64 >,
            Field<STR_LITERAL("replicators"), std::vector<std::string> >,
            Field<STR_LITERAL("offboardingReplicators"), std::vector<std::string> > >;

    using DriveDto = VariadicStruct<
            Field<STR_LITERAL("drive"), DriveDataDto > >;

	using MultipleDrivesDto = std::vector<DriveDto>;

}
