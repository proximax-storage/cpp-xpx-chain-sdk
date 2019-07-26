/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include "multiple_blocks_dto.h"
#include <infrastructure/json/parser.h>
#include <nemcpp/exceptions.h>

using namespace nem2_sdk::internal::dto;

using Parser = nem2_sdk::internal::json::Parser;

MultipleBlocksDto MultipleBlocksDto::from_json(const std::string& jsonStr) {
    MultipleBlocksDtoT dto;
    auto result = Parser::Read(dto, jsonStr);


    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }
    
    MultipleBlocksDto data = MultipleBlocksDto::getFromDto(dto);


    return data;
}

MultipleBlocksDto MultipleBlocksDto::getFromDto(const MultipleBlocksDtoT& dto) {
	MultipleBlocksDto mbdto;

	for (auto& blockDtoT: dto) {
		Block block = BlockDto::getFromDto(blockDtoT);
		mbdto.blocks.push_back(block);
	}

	return mbdto;
}
