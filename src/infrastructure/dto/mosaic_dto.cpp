/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include <infrastructure/json/parser.h>
#include "mosaic_dto.h"

using namespace nem2_sdk;
using namespace nem2_sdk::internal::dto;
using nem2_sdk::internal::json::Parser;



Mosaic MosaicDto::from_json(const std::string& jsonStr) {
    MosaicDtoT dto;
    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

    Mosaic mosaic = MosaicDto::getFromDto(dto);
    return mosaic;
}

Mosaic MosaicDto::getFromDto(const MosaicDtoT& dto) {
    return {dto.value<"id"_>(), dto.value<"amount"_>()};
}


MosaicMeta MosaicMetaDto::from_json(const std::string& jsonStr) {
    MosaicMetaDtoT dto;
    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

    MosaicMeta mosaicMeta = MosaicMetaDto::getFromDto(dto);

    return mosaicMeta;
}


MosaicMeta MosaicMetaDto::getFromDto(const MosaicMetaDtoT& dto) {
    MosaicMeta mosaicMeta = {
            dto.value<"active"_>(),
            dto.value<"index"_>(),
            dto.value<"id"_>()
    };

    return mosaicMeta;
}


MosaicData MosaicDataDto::from_json(const std::string& jsonStr) {
    MosaicDataDtoT dto;
    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

    MosaicData mosaicData = MosaicDataDto::getFromDto(dto);

    return mosaicData;
}

MosaicData MosaicDataDto::getFromDto(const MosaicDataDtoT& dto) {
    MosaicData mosaicData;
    mosaicData.mosaicId = dto.value<"mosaicid"_>();
    mosaicData.amount = dto.value<"supply"_>();
    mosaicData.height = dto.value<"height"_>();
    mosaicData.owner = dto.value<"owner"_>();
    for(auto property : dto.value<"properties"_>()) {
        nem2_sdk::MosaicProperty tmp = {MosaicPropertyId(property[0]), property[1]};
        mosaicData.properties.push_back(tmp);
    }
    return mosaicData;
}

MosaicInfo MosaicInfoDto::from_json(const std::string& jsonStr) {
    MosaicInfoDtoT dto;
    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

    MosaicInfo mosaicInfo = MosaicInfoDto::getFromDto(dto);

    return mosaicInfo;
}

MosaicInfo MosaicInfoDto::getFromDto(const MosaicInfoDtoT &dto) {
    MosaicInfo mosaicInfo = {
            MosaicMetaDto::getFromDto(dto.value<"meta"_>()),
            MosaicDataDto::getFromDto(dto.value<"mosaic"_>())
    };
    return mosaicInfo;
}


MultipleMosaicDto MultipleMosaicDto::from_json(const std::string& jsonStr) {
    MultipleMosaicDtoT dto;
    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

    MultipleMosaicDto multipleMosaicDto = MultipleMosaicDto::getFromDto(dto);

    return multipleMosaicDto;
}

MultipleMosaicDto MultipleMosaicDto::getFromDto(const MultipleMosaicDtoT& dto) {
    MultipleMosaicDto mmdto;

    for (auto& mosaicDtoT: dto) {
        MosaicInfo mosaic = MosaicInfoDto::getFromDto(mosaicDtoT);
        mmdto.mosaics.push_back(mosaic);
    }

    return mmdto;
}

MosaicName MosaicNameDto::from_json(const std::string& jsonStr) {
    MosaicNameDtoT dto;
    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

    MosaicName mosaicName = MosaicNameDto::getFromDto(dto);

    return mosaicName;
}

MosaicName MosaicNameDto::getFromDto(const MosaicNameDtoT& dto) {

    MosaicName mosaicName =  {
            dto.value<"parentid"_>(),
            dto.value<"mosaicid"_>(),
            dto.value<"name"_>()
    };
    return mosaicName;
}

MosaicNamesDto MosaicNamesDto::from_json(const std::string& jsonStr) {
    MosaicNamesDtoT dto;
    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

    MosaicNamesDto mosaicNamesDto = MosaicNamesDto::getFromDto(dto);

    return mosaicNamesDto;
}

MosaicNamesDto MosaicNamesDto::getFromDto(const MosaicNamesDtoT& dto) {
    MosaicNamesDto names;

    for (auto& nameDtoT: dto) {
        MosaicName mosaic = MosaicNameDto::getFromDto(nameDtoT);
        names.names.push_back(mosaic);
    }

    return names;
}