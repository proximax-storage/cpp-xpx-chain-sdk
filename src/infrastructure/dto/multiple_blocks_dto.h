#pragma once

#include "block_dto.h"

#include <vector>

namespace nem2_sdk::internal::dto {
	class MultipleBlocksDto {
	public:
		std::vector<Block> blocks;

		static MultipleBlocksDto from_json(std::istream& json);
		static MultipleBlocksDto from_ptree(boost::property_tree::ptree &pTree);
	};
}
