#pragma once

#include "block_dto.h"

#include <vector>

namespace nem2_sdk::internal::dto {

    using MultipleBlocksDtoT = std::vector<BlockDtoT>;


	class MultipleBlocksDto {
	public:
		std::vector<Block> blocks;

		static MultipleBlocksDto from_json(std::istream& json);
		static MultipleBlocksDto getDto(const MultipleBlocksDtoT & dto);
	};
}
