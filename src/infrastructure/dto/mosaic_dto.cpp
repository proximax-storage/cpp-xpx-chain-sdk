//
// Created by vulfke on 22.07.19.
//

#include <infrastructure/json/parser.h>
#include "mosaic_dto.h"

using namespace nem2_sdk;
using namespace nem2_sdk::internal::dto;
using nem2_sdk::internal::json::Parser;


MosaicMeta MosaicMetaDto::from_json(const std::string& jsonStr) {
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


MosaicData MosaicDataDto::from_json(const std::string& jsonStr) {
    MosaicDataDtoT dto;
    Parser::Read(dto, jsonStr);

    MosaicDataDto mosaicDto = MosaicDataDto::getDto(dto);

    return mosaicDto;
}

MosaicDataDto MosaicDataDto::getDto(const MosaicDataDtoT& dto) {
    MosaicDataDto mosaicDto;
    mosaicDto.mosaicId = dto.value<"mosaicid"_>();
    mosaicDto.amount = dto.value<"supply"_>();
    mosaicDto.height = dto.value<"height"_>();
    mosaicDto.owner = dto.value<"owner"_>();
    for(auto property : dto.value<"properties"_>()) {
        nem2_sdk::MosaicProperty tmp = {MosaicPropertyId(property[0]), property[1]};
        mosaicDto.properties.push_back(tmp);
    }
    return mosaicDto;
}

MosaicInfo MosaicInfoDto::from_json(const std::string& jsonStr) {
    MosaicInfoDtoT dto;
    Parser::Read(dto, jsonStr);

    // TODO: Error Processing

    MosaicInfoDto mosaicDto = MosaicInfoDto::getDto(dto);

    return mosaicDto;
}

MosaicInfoDto MosaicInfoDto::getDto(const MosaicInfoDtoT &dto) {
    MosaicInfoDto mosaicDto = {
            MosaicMetaDto::getDto(dto.value<"meta"_>()),
            MosaicDataDto::getDto(dto.value<"mosaic"_>())
    };
    return mosaicDto;
}


MultipleMosaicDto MultipleMosaicDto::from_json(const std::string& jsonStr) {
    MultipleMosaicDtoT dto;
    Parser::Read(dto, jsonStr);

    // TODO: Error Processing

    MultipleMosaicDto multipleMosaicDto = MultipleMosaicDto::getDto(dto);

    return multipleMosaicDto;
}

MultipleMosaicDto MultipleMosaicDto::getDto(const MultipleMosaicDtoT& dto) {
    MultipleMosaicDto mmdto;

    for (auto& mosaicDtoT: dto) {
        MosaicInfo mosaic = MosaicInfoDto::getDto(mosaicDtoT);
        mmdto.mosaics.push_back(mosaic);
    }

    return mmdto;
}

MosaicName MosaicNameDto::from_json(const std::string& jsonStr) {
    MosaicNameDtoT dto;
    Parser::Read(dto, jsonStr);

    // TODO: Error Processing

    MosaicNameDto mosaicNameDto = MosaicNameDto::getDto(dto);

    return mosaicNameDto;
}

MosaicNameDto MosaicNameDto::getDto(const MosaicNameDtoT& dto) {

    MosaicNameDto mosaicName =  {
            dto.value<"parentid"_>(),
            dto.value<"mosaicid"_>(),
            dto.value<"name"_>()
    };
    return mosaicName;
}

MosaicNamesDto MosaicNamesDto::from_json(const std::string& jsonStr) {
    MosaicNamesDtoT dto;
    Parser::Read(dto, jsonStr);

    // TODO: Error Processing

    MosaicNamesDto mosaicNamesDto = MosaicNamesDto::getDto(dto);

    return mosaicNamesDto;
}

MosaicNamesDto MosaicNamesDto::getDto(const MosaicNamesDtoT& dto) {
    MosaicNamesDto names;

    for (auto& nameDtoT: dto) {
        MosaicName mosaic = MosaicNameDto::getDto(nameDtoT);
        names.names.push_back(mosaic);
    }

    return names;
}