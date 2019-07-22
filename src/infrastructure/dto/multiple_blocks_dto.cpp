#include "multiple_blocks_dto.h"
#include <infrastructure/json/parser.h>
using namespace nem2_sdk::internal::dto;

using Parser = nem2_sdk::internal::json::Parser;

MultipleBlocksDto MultipleBlocksDto::from_json(const std::string& jsonStr) {
    MultipleBlocksDtoT dto;
    Parser::Read(dto, jsonStr);

    // TODO: Error Processing

    MultipleBlocksDto data = MultipleBlocksDto::getDto(dto);

    return data;
}

MultipleBlocksDto MultipleBlocksDto::getDto(const MultipleBlocksDtoT& dto) {
	MultipleBlocksDto mbdto;

	for (auto& blockDtoT: dto) {
		Block block = BlockDto::getDto(blockDtoT);
		mbdto.blocks.push_back(block);
	}

	return mbdto;
}
