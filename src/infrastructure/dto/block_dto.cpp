/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include "block_dto.h"
#include <nemcpp/client.h>
#include <infrastructure/json/parser.h>

#include <iostream>

using namespace nem2_sdk;
using namespace nem2_sdk::internal::dto;

using Parser = nem2_sdk::internal::json::Parser;


BlockMeta BlockMetaDto::from_json(const std::string& jsonStr) {
	BlockMetaDtoT dto;

    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

	BlockMeta meta = BlockMetaDto::getFromDto(dto);

	return meta;
}

BlockMeta BlockMetaDto::getFromDto(const BlockMetaDtoT &dto) {
    BlockMeta meta;
    meta.hash = dto.value<"hash"_>();
    meta.generationHash = dto.value<"generationHash"_>();
    meta.totalFee = dto.value<"totalFee"_>();  // note how Uint64 type from dto ([high, low] in json automatically converted to uint64_t
    meta.numTransactions = dto.value<"numTransactions"_>();
    return meta;
}

BlockData BlockDataDto::from_json(const std::string& jsonStr) {
    BlockDataDtoT dto;

    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

    BlockData data = BlockDataDto::getFromDto(dto);

    return data;
}

BlockData BlockDataDto::getFromDto(const BlockDataDtoT& dto) {
    BlockData data = {
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

Block BlockDto::from_json(const std::string& jsonStr) {
    BlockDtoT dto;
    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

    Block block = BlockDto::getFromDto(dto);

    return block;
}

Block BlockDto::getFromDto(const BlockDtoT &dto) {
    Block block = {
            BlockMetaDto::getFromDto(dto.value<"meta"_>()),
            BlockDataDto::getFromDto(dto.value<"block"_>())
    };
    return block;
}

StorageInfo StorageInfoDto::from_json(const std::string& jsonStr) {
    StorageInfoDtoT dto;
    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

    StorageInfo storageInfoDto = StorageInfoDto::getFromDto(dto);

    return storageInfoDto;
}

StorageInfo StorageInfoDto::getFromDto(const StorageInfoDtoT& dto) {

    StorageInfo storageInfo = {
            dto.value<"numBlocks"_>(),
            dto.value<"numTransactions"_>(),
            dto.value<"numAccounts"_>()
    };

    return storageInfo;
}


ScoreInfo ScoreInfoDto::from_json(const std::string& jsonStr) {
    ScoreInfoDtoT dto;
    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

    ScoreInfo scoreInfo = ScoreInfoDto::getFromDto(dto);

    return scoreInfo;
}

ScoreInfo ScoreInfoDto::getFromDto(const ScoreInfoDtoT& dto) {
    ScoreInfo scoreInfo = {
            dto.value<"scoreHigh"_>(),
            dto.value<"scoreLow"_>()
    };

    return scoreInfo;
}
