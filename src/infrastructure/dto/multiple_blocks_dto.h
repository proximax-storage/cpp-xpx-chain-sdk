/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/
#pragma once

#include "block_dto.h"

#include <vector>


namespace nem2_sdk::internal::dto {

    using MultipleBlocksDtoT = std::vector<BlockDtoT>;


	class MultipleBlocksDto {
	public:
		std::vector<Block> blocks;

		static MultipleBlocksDto from_json(const std::string& jsonStr);
		static MultipleBlocksDto getFromDto(const MultipleBlocksDtoT & dto);
	};
}
