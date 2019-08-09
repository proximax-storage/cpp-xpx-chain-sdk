#include "multiple_blocks_dto.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace nem2_sdk::internal::dto;

MultipleBlocksDto MultipleBlocksDto::from_json(std::istream& json) {
	boost::property_tree::ptree pTree;
	boost::property_tree::read_json(json, pTree);

	return MultipleBlocksDto::from_ptree(pTree);
}

MultipleBlocksDto MultipleBlocksDto::from_ptree(boost::property_tree::ptree& pTree) {
	MultipleBlocksDto dto;

	for (auto& child: pTree) {
		auto block = BlockDto::from_ptree(child.second);
		dto.blocks.push_back(block);
	}

	return dto;
}
