//
// Created by vulfke on 22.07.19.
//

#include <infrastructure/json/parser.h>
#include "mosaic_dto.h"

using namespace nem2_sdk::internal::dto;
using nem2_sdk::internal::json::Parser;


MosaicMetaDto MosaicMetaDto::from_json(const std::string& jsonStr) {
    MosaicMetaDtoT dto;
    Parser::Read(dto, jsonStr);

    MosaicMetaDto mosaicDto = MosaicMetaDto::getDto(dto);

    return mosaicDto;
}


MosaicMetaDto MosaicMetaDto::getDto(const MosaicMetaDtoT& dto) {
    MosaicMetaDto mosaicMetaDto = {
            dto.value<"active"_>(),
            dto.value<"index"_>(),
            dto.value<"id"_>()
    };

    return mosaicMetaDto;
}


MosaicDataDto MosaicDataDto::from_json(const std::string& jsonStr) {
    MosaicDataDtoT dto;
    Parser::Read(dto, jsonStr);

    MosaicDataDto mosaicDto = MosaicDataDto::getDto(dto);

    return mosaicDto;
}

MosaicDataDto MosaicDataDto::getDto(const MosaicDataDtoT& dto) {
    MosaicDataDto mosaicDto = {
            dto.value<"mosaicid"_>(),
            dto.value<"supply"_>(),
            dto.value<"height"_>(),
            dto.value<"owner"_>(),

    };
    return mosaicDto;
}

MosaicDto MosaicDto::from_json(const std::string& jsonStr) {
    MosaicDtoT dto;
    Parser::Read(dto, jsonStr);

    // TODO: Error Processing

    MosaicDto blockDto = MosaicDto::getDto(dto);

    return blockDto;
}

MosaicDto MosaicDto::getDto(const MosaicDtoT &dto) {
    MosaicDto mosaicDto = {
            MosaicMetaDto::getDto(dto.value<"meta"_>()),
            MosaicDataDto::getDto(dto.value<"mosaic"_>())
    };
    return mosaicDto;
}