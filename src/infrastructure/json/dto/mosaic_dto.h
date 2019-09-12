
/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/
#pragma once

#include <xpxchaincpp/model/mosaic/mosaic.h>
#include <xpxchaincpp/model/mosaic/mosaic_property.h>
#include <infrastructure/utils/variadic_struct.h>
#include <infrastructure/json/uint64.h>
#include <xpxchaincpp/model/mosaic/mosaic.h>
#include <xpxchaincpp/model/mosaic/mosaic_property.h>
#include <stdint.h>

namespace xpx_chain_sdk::internal::json::dto {
    using internal::json::Uint64;

    using MyMosaicPropertyDto = VariadicStruct<
            Field<STR_LITERAL("id"), uint8_t >,
            Field<STR_LITERAL("value"), Uint64> >;

    using MosaicDto = VariadicStruct<
            Field<STR_LITERAL("id"), Uint64>,
            Field<STR_LITERAL("amount"), Uint64> >;

    using MosaicMetaDto = VariadicStruct<
            Field<STR_LITERAL("id"), std::string> >;

    using MosaicDataDto = VariadicStruct<
            Field<STR_LITERAL("mosaicId"), Uint64>,
            Field<STR_LITERAL("supply"), Uint64>,
            Field<STR_LITERAL("height"), Uint64>,
            Field<STR_LITERAL("owner"), std::string>,
            Field<STR_LITERAL("properties"), std::vector<MyMosaicPropertyDto> > >;

    using MosaicInfoDto = VariadicStruct<
            Field<STR_LITERAL("meta"), MosaicMetaDto>,
            Field<STR_LITERAL("mosaic"), MosaicDataDto> >;


    using MultipleMosaicInfoDto = std::vector<MosaicInfoDto>;
    using MultipleMosaicDto = std::vector<MosaicDto>;

    using MosaicNameDto = VariadicStruct<
            Field<STR_LITERAL("parentid"), Uint64>,
            Field<STR_LITERAL("mosaicid"), Uint64>,
            Field<STR_LITERAL("name"), std::string> >;

    using MosaicNamesDto = std::vector<MosaicNameDto>;
}

