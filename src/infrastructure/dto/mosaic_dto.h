//
// Created by vulfke on 22.07.19.
//

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

    using MosaicDtoT = VariadicStruct<
        Field<STR_LITERAL("meta"), MosaicMetaDtoT>,
        Field<STR_LITERAL("mosaic"), MosaicDataDtoT> >;


    using MultipleMosaicDtoT = std::vector<MosaicDtoT>;

    using MosaicNameDtoT = VariadicStruct<
        Field<STR_LITERAL("parentid"), Uint64>,
        Field<STR_LITERAL("mosaicid"), Uint64>,
        Field<STR_LITERAL("name"), std::string> >;

    using MosaicNamesDtoT = std::vector<MosaicNameDtoT>;

    class MosaicMetaDto : public MosaicMeta {
    public:
        static MosaicMeta from_json(const std::string& jsonStr);
        static MosaicMetaDto getDto(const MosaicMetaDtoT& dto);
    };


    class MosaicDataDto : public MosaicData {
    public:
        static MosaicData from_json(const std::string& jsonStr);
        static MosaicDataDto getDto(const MosaicDataDtoT& dto);
    };


    class MosaicDto : public RichMosaic {
    public:
        static RichMosaic from_json(const std::string& jsonStr);
        static MosaicDto getDto(const MosaicDtoT& dto);
    };

    class MultipleMosaicDto {
    public:
        std::vector<RichMosaic> mosaics;

        static MultipleMosaicDto from_json(const std::string& jsonStr);
        static MultipleMosaicDto getDto(const MultipleMosaicDtoT& dto);
    };

    class MosaicNameDto : public MosaicName {
    public:
        static MosaicName from_json(const std::string& jsonStr);
        static MosaicNameDto getDto(const MosaicNameDtoT& dto);
    };


    class MosaicNamesDto {
    public:
        std::vector<MosaicName> names;

        static MosaicNamesDto from_json(const std::string& jsonStr);
        static MosaicNamesDto getDto(const MosaicNamesDtoT& dto);
    };

}
