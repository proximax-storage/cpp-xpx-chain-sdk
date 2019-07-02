
#pragma once

#include "infrastructure/json/uint64.h"

namespace nem2_sdk { namespace internal { namespace json {
	
	using MosaicDTO = VariadicStruct<
		Field<STR_LITERAL("id"),     Uint64>,
		Field<STR_LITERAL("amount"), Uint64>>;
	
	using MosaicPropertyDTO = VariadicStruct<
		Field<STR_LITERAL("id"),    MosaicPropertyId>,
		Field<STR_LITERAL("value"), Uint64>>;
}}}
