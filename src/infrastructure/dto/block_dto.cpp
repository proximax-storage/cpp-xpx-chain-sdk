#include "block_dto.h"
#include "lower_higher.h"
#include <nemcpp/client.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace nem2_sdk;
using namespace nem2_sdk::internal::dto;

BlockMetaDto BlockMetaDto::from_json(std::istream& json) {
	boost::property_tree::ptree pTree;
	boost::property_tree::read_json(json, pTree);

	return BlockMetaDto::from_ptree(pTree);
}

BlockMetaDto BlockMetaDto::from_ptree(boost::property_tree::ptree &pTree) {
	try {
		auto hash = pTree.get_child("hash").get_value<std::string>();
		auto generationHash = pTree.get_child("generationHash").get_value<std::string>();
		auto totalFee = internal::dto::LowerHigherDto::from_ptree(
				pTree.get_child("totalFee")
		).higher;
		auto numTransactions = pTree.get_child("numTransactions").get_value<uint64_t>();

		BlockMetaDto dto{hash, generationHash, totalFee, numTransactions};

		return dto;
	} catch (boost::property_tree::ptree_bad_path& e) {
		throw InvalidJSON(e);
	}
}

BlockDataDto BlockDataDto::from_json(std::istream& json) {
	boost::property_tree::ptree pTree;
	boost::property_tree::read_json(json, pTree);

	return BlockDataDto::from_ptree(pTree);
}

BlockDataDto BlockDataDto::from_ptree(boost::property_tree::ptree &pTree) {
	try {
		auto signature = pTree.get_child("signature").get_value<std::string>();
		auto timestamp = internal::dto::LowerHigherDto::from_ptree(
				pTree.get_child("timestamp")
		).higher;
		auto difficulty = internal::dto::LowerHigherDto::from_ptree(
				pTree.get_child("difficulty")
		).higher;
		auto feeMultiplier = pTree.get_child("feeMultiplier").get_value<uint64_t>();
		auto previousBlockHash = pTree.get_child("previousBlockHash").get_value<std::string>();
		auto blockTransactionsHash = pTree.get_child("blockTransactionsHash").get_value<std::string>();
		auto blockReceiptsHash = pTree.get_child("blockReceiptsHash").get_value<std::string>();
		auto stateHash = pTree.get_child("stateHash").get_value<std::string>();
		auto beneficiaryPublicKey = pTree.get_child("beneficiary").get_value<std::string>();

		BlockDataDto dto{
			signature,
			timestamp,
			difficulty,
			feeMultiplier,
			previousBlockHash,
			blockTransactionsHash,
			blockReceiptsHash,
			stateHash,
			beneficiaryPublicKey
		};

		return dto;
	} catch (boost::property_tree::ptree_bad_path& e) {
		throw InvalidJSON(e);
	}
}

BlockDto BlockDto::from_json(std::istream& json) {
	boost::property_tree::ptree pTree;
	boost::property_tree::read_json(json, pTree);

	return BlockDto::from_ptree(pTree);
}

BlockDto BlockDto::from_ptree(boost::property_tree::ptree &pTree) {
	BlockDto dto{
		BlockMetaDto::from_ptree(pTree.get_child("meta")),
		BlockDataDto::from_ptree(pTree.get_child("block"))
	};

	return dto;
}
