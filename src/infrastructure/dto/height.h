#pragma once

#include <string>
#include <infrastructure/json/uint64.h>
#include <boost/property_tree/ptree.hpp>
#include "infrastructure/utils/variadic_struct.h"

#include <stdint.h>
namespace nem2_sdk::internal::dto {
    using nem2_sdk::internal::json::Uint64;
    using HeightDtoT = VariadicStruct<Field<STR_LITERAL("height"), Uint64> >;

	class HeightDto {
	public:
		uint64_t height;
		static HeightDto from_json(const std::string& jsonStr);
		static HeightDto getDto(const HeightDtoT& dto);
	};
}
