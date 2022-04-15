/*
*** Copyright 2022 ProximaX Limited. All rights reserved.
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

    using RateDataDto = VariadicStruct<
            Field<STR_LITERAL("currencyAmount"), Uint64>,
            Field<STR_LITERAL("mosaicAmount"), Uint64> >;

    using TurnoverDataDto = VariadicStruct<
            Field<STR_LITERAL("rate"), RateDataDto>,
            Field<STR_LITERAL("turnover"), Uint64>>;

    using LiquidityProviderDataDto = VariadicStruct<
            Field<STR_LITERAL("mosaicId"), Uint64>,
            Field<STR_LITERAL("providerKey"), std::string>,
            Field<STR_LITERAL("owner"), std::string>,
            Field<STR_LITERAL("additionallyMinted"), Uint64>,
            Field<STR_LITERAL("slashingAccount"), std::string >,
            Field<STR_LITERAL("slashingPeriod"), Uint32 >,
            Field<STR_LITERAL("windowSize"), uint16_t >,
            Field<STR_LITERAL("creationHeight"), Uint64 >,
            Field<STR_LITERAL("alpha"), Uint32 >,
            Field<STR_LITERAL("beta"), Uint32 >,
            Field<STR_LITERAL("recentTurnover"), TurnoverDataDto >,
            Field<STR_LITERAL("turnoverHistory"), std::vector<TurnoverDataDto> > >;

    using LiquidityProviderDto = VariadicStruct<
            Field<STR_LITERAL("liquidityProvider"), LiquidityProviderDataDto > >;

	using MultipleLiquidityProvidersDto = std::vector<LiquidityProviderDto>;
}
