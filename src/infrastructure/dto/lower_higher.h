#pragma once

#include <string>

namespace nem2_sdk::internal::dto {
	class LowerHigherDto {
	public:
		uint64_t lower, higher;
		static LowerHigherDto from_json(std::istream& json);
	};
}
