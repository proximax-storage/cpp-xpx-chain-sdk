#include "multiple_blocks_dto.h"

//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/json_parser.hpp>
#include <infrastructure/json/parser.h>
using namespace nem2_sdk::internal::dto;

using Parser = nem2_sdk::internal::json::Parser;

MultipleBlocksDto MultipleBlocksDto::from_json(std::istream& json) {
    std::string jsonStr;
    std::getline(json, jsonStr);
    MultipleBlocksDtoT dto;
    Parser::Read(dto, jsonStr);
    MultipleBlocksDto data = MultipleBlocksDto::getDto(dto);
    return data;
}

MultipleBlocksDto MultipleBlocksDto::getDto(const MultipleBlocksDtoT& dto) {
	MultipleBlocksDto mbdto;

	for (auto& blockDtoT: dto.value<"blocksDtoT"_>()) {
		Block block = BlockDto::getDto(blockDtoT);
		mbdto.blocks.push_back(block);
	}

	return mbdto;
}
