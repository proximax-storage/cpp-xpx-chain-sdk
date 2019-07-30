/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/
#pragma once

#include <nemcpp/model/mosaic/mosaic.h>
#include <nemcpp/model/mosaic/mosaic_property.h>
#include <infrastructure/utils/variadic_struct.h>
#include <infrastructure/json/uint64.h>
#include <nemcpp/model/mosaic/mosaic.h>
#include <nemcpp/model/mosaic/mosaic_property.h>
#include <stdint.h>

namespace nem2_sdk::internal::dto {
    using internal::json::Uint64;

    using MosaicDtoT = VariadicStruct<
        Field<STR_LITERAL("id"), Uint64>,
        Field<STR_LITERAL("amount"), Uint64> >;

    using MosaicMetaDtoT = VariadicStruct<
        Field<STR_LITERAL("active"), bool>,
        Field<STR_LITERAL("index"), uint64_t>,
        Field<STR_LITERAL("id"), std::string> >;

    using MosaicDataDtoT = VariadicStruct<
        Field<STR_LITERAL("mosaicid"), Uint64>,
        Field<STR_LITERAL("supply"), Uint64>,
        Field<STR_LITERAL("height"), Uint64>,
        Field<STR_LITERAL("owner"), std::string>,
        Field<STR_LITERAL("properties"), std::vector<std::array<uint64_t, 2> > > >;

    using MosaicInfoDtoT = VariadicStruct<
        Field<STR_LITERAL("meta"), MosaicMetaDtoT>,
        Field<STR_LITERAL("mosaic"), MosaicDataDtoT> >;


    using MultipleMosaicDtoT = std::vector<MosaicInfoDtoT>;

    using MosaicNameDtoT = VariadicStruct<
        Field<STR_LITERAL("parentid"), Uint64>,
        Field<STR_LITERAL("mosaicid"), Uint64>,
        Field<STR_LITERAL("name"), std::string> >;

    using MosaicNamesDtoT = std::vector<MosaicNameDtoT>;

    class MosaicDto {
    public:
        static Mosaic from_json(const std::string& jsonStr);
        static Mosaic getFromDto(const MosaicDtoT& dto);
    };

    class MosaicMetaDto {
    public:
        static MosaicMeta from_json(const std::string& jsonStr);
        static MosaicMeta getFromDto(const MosaicMetaDtoT& dto);
    };


    class MosaicDataDto {
    public:
        static MosaicData from_json(const std::string& jsonStr);
        static MosaicData getFromDto(const MosaicDataDtoT& dto);
    };


    class MosaicInfoDto {
    public:
        static MosaicInfo from_json(const std::string& jsonStr);
        static MosaicInfo getFromDto(const MosaicInfoDtoT& dto);
    };

    class MultipleMosaicDto {
    public:
        std::vector<MosaicInfo> mosaics;

        static MultipleMosaicDto from_json(const std::string& jsonStr);
        static MultipleMosaicDto getFromDto(const MultipleMosaicDtoT& dto);
    };

    class MosaicNameDto {
    public:
        static MosaicName from_json(const std::string& jsonStr);
        static MosaicName getFromDto(const MosaicNameDtoT& dto);
    };


    class MosaicNamesDto {
    public:
        std::vector<MosaicName> names;

        static MosaicNamesDto from_json(const std::string& jsonStr);
        static MosaicNamesDto getFromDto(const MosaicNamesDtoT& dto);
    };

}
