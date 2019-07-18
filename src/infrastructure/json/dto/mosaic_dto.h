
#pragma once

#include "infrastructure/json/uint64.h"
#include "infrastructure/utils/variadic_struct.h"

#include <nemcpp/model/mosaic/mosaic_property.h>

namespace nem2_sdk { namespace internal { namespace json {
	
	// See ../parser.h for description of supported variadic struct field types and descriptors.

	using MosaicIdDTO = Uint64;

	using MosaicDTO = VariadicStruct<
		Field<STR_LITERAL("id"),     MosaicIdDTO>,
		Field<STR_LITERAL("amount"), Uint64>>;
	
	using MosaicPropertyDTO = VariadicStruct<
		Field<STR_LITERAL("id"),    MosaicPropertyId>,
		Field<STR_LITERAL("value"), Uint64>>;
}}}
