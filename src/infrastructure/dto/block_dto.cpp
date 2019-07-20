#include "block_dto.h"
#include "lower_higher.h"
#include <nemcpp/client.h>
#include <infrastructure/json/parser.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>

using namespace nem2_sdk;
using namespace nem2_sdk::internal::dto;

using Parser = nem2_sdk::internal::json::Parser;


BlockMetaDto BlockMetaDto::from_json(std::istream& json) {
    std::string jsonStr;
    std::getline(json, jsonStr);
	BlockMetaDtoT dto;
	Parser::Read(dto, jsonStr);
    BlockMetaDto meta = BlockMetaDto::getDto(dto);

	return meta;
}

BlockMetaDto BlockMetaDto::getDto(const BlockMetaDtoT &dto) {
    BlockMetaDto meta;
    meta.hash = dto.value<"hash"_>();
    meta.generationHash = dto.value<"generationHash"_>();
    meta.totalFee = dto.value<"totalFee"_>();  // note how Uint64 type from dto ([high, low] in json automatically converted to uint64_t
    meta.numTransactions = dto.value<"numTransactions"_>();
    return meta;
}


BlockDataDto BlockDataDto::from_json(std::istream& json) {
    std::string jsonStr;
    std::getline(json, jsonStr);
    BlockDataDtoT dto;
    Parser::Read(dto, jsonStr);
    BlockDataDto data = BlockDataDto::getDto(dto);
    return data;
}


BlockDataDto BlockDataDto::getDto(const BlockDataDtoT& dto) {
    BlockDataDto data = {
            dto.value<"signature"_>(),
            dto.value<"timestamp"_>(),
            dto.value<"difficulty"_>(),
            dto.value<"feeMultiplier"_>(),
            dto.value<"previousBlockHash"_>(),
            dto.value<"blockTransactionsHash"_>(),
            dto.value<"blockReceiptsHash"_>(),
            dto.value<"stateHash"_>(),
            dto.value<"beneficiaryPublicKey"_>()
    };
    std::cout << data.signature << ' ' << dto.value<"signature"_>() << std::endl;
    return data;
}

BlockDto BlockDto::from_json(std::istream& json) {
    std::string jsonStr;
    std::getline(json, jsonStr);
    BlockDtoT dto;
    auto result = Parser::Read(dto, jsonStr);
    std::cout << jsonStr << ' ' << result << ' ' << result.invalidField() << std::endl;
    BlockDto blockDto = BlockDto::getDto(dto);

    return blockDto;
}

BlockDto BlockDto::getDto(const BlockDtoT &dto) {
    BlockDto blockDto = {
            BlockMetaDto::getDto(dto.value<"meta"_>()),
            BlockDataDto::getDto(dto.value<"data"_>())
    };
    return blockDto;
}


//BlockDataDto BlockDataDto::from_json(std::istream& json) {
//	boost::property_tree::ptree pTree;
//	boost::property_tree::read_json(json, pTree);
//
//	return BlockDataDto::from_ptree(pTree);
//}
//
//BlockDataDto BlockDataDto::from_ptree(boost::property_tree::ptree &pTree) {
//	try {
//		auto signature = pTree.get_child("signature").get_value<std::string>();
//		auto timestamp = internal::dto::LowerHigherDto::from_ptree(
//				pTree.get_child("timestamp")
//		).higher;
//		auto difficulty = internal::dto::LowerHigherDto::from_ptree(
//				pTree.get_child("difficulty")
//		).higher;
//		auto feeMultiplier = pTree.get_child("feeMultiplier").get_value<uint64_t>();
//		auto previousBlockHash = pTree.get_child("previousBlockHash").get_value<std::string>();
//		auto blockTransactionsHash = pTree.get_child("blockTransactionsHash").get_value<std::string>();
//		auto blockReceiptsHash = pTree.get_child("blockReceiptsHash").get_value<std::string>();
//		auto stateHash = pTree.get_child("stateHash").get_value<std::string>();
//		auto beneficiaryPublicKey = pTree.get_child("beneficiary").get_value<std::string>();
//
//		BlockDataDto dto{
//			signature,
//			timestamp,
//			difficulty,
//			feeMultiplier,
//			previousBlockHash,
//			blockTransactionsHash,
//			blockReceiptsHash,
//			stateHash,
//			beneficiaryPublicKey
//		};
//
//		return dto;
//	} catch (boost::property_tree::ptree_bad_path& e) {
//		throw InvalidJSON(e);
//	}
//}

//BlockDto BlockDto::from_json(std::istream& json) {
//	boost::property_tree::ptree pTree;
//	boost::property_tree::read_json(json, pTree);
//
//	return BlockDto::from_ptree(pTree);
//}
//
//BlockDto BlockDto::from_ptree(boost::property_tree::ptree &pTree) {
//	BlockDto dto{
//		BlockMetaDto::from_ptree(pTree.get_child("meta")),
//		BlockDataDto::from_ptree(pTree.get_child("block"))
//	};
//
//	return dto;
//}
