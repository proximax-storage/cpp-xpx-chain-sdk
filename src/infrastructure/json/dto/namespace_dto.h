/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <string>
#include <stdint.h>
#include <vector>
#include <infrastructure/utils/variadic_struct.h>
#include <infrastructure/json/uint64.h>
#include <nemcpp/model/namespace/namespace_info.h>


namespace xpx_sdk::internal::json::dto {

    using xpx_sdk::internal::json::Uint64;

    using AliasDto = VariadicStruct<
            Field<STR_LITERAL("type"), uint64_t>,
            Field<STR_LITERAL("mosaicId"), std::string>,
            Field<STR_LITERAL("address"), std::string> >;

    using NamespaceMetaDto = VariadicStruct<
            Field<STR_LITERAL("active"), bool>,
            Field<STR_LITERAL("index"), uint64_t>,
            Field<STR_LITERAL("id"), std::string> >;

    using NamespaceDataDto = VariadicStruct<
            Field<STR_LITERAL("owner"), std::string>,
            Field<STR_LITERAL("ownerAddress"), std::string>,
            Field<STR_LITERAL("startHeight"), Uint64>,
            Field<STR_LITERAL("endHeight"), Uint64>,
            Field<STR_LITERAL("depth"), uint64_t>,
            Field<STR_LITERAL("level0"), Uint64>,
            Field<STR_LITERAL("level1"), Uint64>,
            Field<STR_LITERAL("level2"), Uint64>,
            Field<STR_LITERAL("type"), uint64_t>,
            Field<STR_LITERAL("alias"), AliasDto>,
            Field<STR_LITERAL("parentId"), Uint64> >;

    using NamespaceInfoDto = VariadicStruct<
            Field<STR_LITERAL("meta"), NamespaceMetaDto>,
            Field<STR_LITERAL("namespace"), NamespaceDataDto> >;

    using NamespaceNameDto = VariadicStruct<
            Field<STR_LITERAL("name"), std::string> >;

    using NamespaceNamesDto = std::vector<NamespaceNameDto>;



}