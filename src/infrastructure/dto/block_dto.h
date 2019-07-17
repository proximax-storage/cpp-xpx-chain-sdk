#pragma once

#include <nemcpp/model/blockchain/block.h>

#include <boost/property_tree/ptree.hpp>

namespace nem2_sdk::internal::dto {
	class BlockMetaDto : public BlockMeta {
	public:
		static BlockMetaDto from_json(std::istream& json);
		static BlockMetaDto from_ptree(boost::property_tree::ptree &pTree);
	};

	class BlockDataDto : public BlockData {
	public:
		static BlockDataDto from_json(std::istream& json);
		static BlockDataDto from_ptree(boost::property_tree::ptree &pTree);
	};

	class BlockDto : public Block {
	public:
		static BlockDto from_json(std::istream& json);
		static BlockDto from_ptree(boost::property_tree::ptree &pTree);
	};
}
