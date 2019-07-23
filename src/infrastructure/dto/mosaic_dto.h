//
// Created by vulfke on 22.07.19.
//

#pragma once

#include <nemcpp/model/mosaic/mosaic.h>
#include <nemcpp/model/mosaic/mosaic_property.h>
#include <infrastructure/utils/variadic_struct.h>
#include <infrastructure/json/uint64.h>
#include <stdint.h>

namespace nem2_sdk::internal::dto {
    using internal::json::Uint64;

    using MosaicMetaDtoT = VariadicStruct<
            Field<STR_LITERAL("active"), bool>,
            Field<STR_LITERAL("index"), uint64_t>,
            Field<STR_LITERAL("id"), std::string> >;

    class MosaicMetaDto {
    public:
        bool active;
        uint64_t index;
        std::string id;
    public:
        static MosaicMetaDto from_json(const std::string& jsonStr);
        static MosaicMetaDto getDto(const MosaicMetaDtoT& dto);
    };

    using MosaicDataDtoT = VariadicStruct<
            Field<STR_LITERAL("mosaicid"), Uint64>,
            Field<STR_LITERAL("supply"), Uint64>,
            Field<STR_LITERAL("height"), Uint64>,
            Field<STR_LITERAL("owner"), std::string>,
            Field<STR_LITERAL("properties"), std::vector<std::array<uint64_t, 2> > > >;

    class MosaicDataDto {
    public:
        uint64_t mosaicId;
        uint64_t supply;
        uint64_t height;
        std::string owner;
        MosaicPropertyContainer properties;

    public:
        static MosaicDataDto from_json(const std::string& jsonStr);
        static MosaicDataDto getDto(const MosaicDataDtoT& dto);
    };

    using MosaicDtoT = VariadicStruct<
            Field<STR_LITERAL("meta"), MosaicMetaDtoT>,
            Field<STR_LITERAL("mosaic"), MosaicDataDtoT> >;

    class MosaicDto {
    public:
        MosaicMetaDto meta;
        MosaicDataDto data;
    public:
        static MosaicDto from_json(const std::string& jsonStr);
        static MosaicDto getDto(const MosaicDtoT& dto);
    };

}
