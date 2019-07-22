#include "block_dto.h"
#include "lower_higher.h"
#include <nemcpp/client.h>
#include <infrastructure/json/parser.h>

#include <iostream>

using namespace nem2_sdk;
using namespace nem2_sdk::internal::dto;

using Parser = nem2_sdk::internal::json::Parser;


BlockMetaDto BlockMetaDto::from_json(std::istream& json) {
    std::string jsonStr;
    std::getline(json, jsonStr);

	BlockMetaDtoT dto;
	Parser::Read(dto, jsonStr);

    // TODO: Error Processing

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

    // TODO: Error Processing

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
            dto.value<"beneficiary"_>()
    };
    return data;
}

BlockDto BlockDto::from_json(std::istream& json) {
    std::string jsonStr;
    std::getline(json, jsonStr);

    BlockDtoT dto;
    Parser::Read(dto, jsonStr);

    // TODO: Error Processing

    BlockDto blockDto = BlockDto::getDto(dto);

    return blockDto;
}

BlockDto BlockDto::getDto(const BlockDtoT &dto) {
    BlockDto blockDto = {
            BlockMetaDto::getDto(dto.value<"meta"_>()),
            BlockDataDto::getDto(dto.value<"block"_>())
    };
    return blockDto;
}
