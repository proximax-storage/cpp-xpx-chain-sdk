/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#pragma once

#include <string>
#include <stdint.h>
#include <infrastructure/utils/variadic_struct.h>
#include <infrastructure/json/uint64.h>
#include <nemcpp/model/namespace/namespace_info.h>


namespace nem2_sdk::internal::dto {

    using nem2_sdk::internal::json::Uint64;

    using AliasDtoT = VariadicStruct<
            Field<STR_LITERAL("type"), uint64_t>,
            Field<STR_LITERAL("mosaicId"), std::string>,
            Field<STR_LITERAL("address"), std::string> >;

    using NamespaceMetaDtoT = VariadicStruct<
            Field<STR_LITERAL("active"), bool>,
            Field<STR_LITERAL("index"), uint64_t>,
            Field<STR_LITERAL("id"), std::string> >;

    using NamespaceDataDtoT = VariadicStruct<
            Field<STR_LITERAL("owner"), std::string>,
            Field<STR_LITERAL("ownerAddress"), std::string>,
            Field<STR_LITERAL("startHeight"), Uint64>,
            Field<STR_LITERAL("endHeight"), Uint64>,
            Field<STR_LITERAL("depth"), uint64_t>,
            Field<STR_LITERAL("level0"), Uint64>,
            Field<STR_LITERAL("level1"), Uint64>,
            Field<STR_LITERAL("level2"), Uint64>,
            Field<STR_LITERAL("type"), uint64_t>,
            Field<STR_LITERAL("alias"), AliasDtoT>,
            Field<STR_LITERAL("parentId"), Uint64> >;

    using NamespaceInfoDtoT = VariadicStruct<
            Field<STR_LITERAL("meta"), NamespaceMetaDtoT>,
            Field<STR_LITERAL("namespace"), NamespaceDataDtoT> >;


    class AliasDto : public Alias {
    public:
        static Alias from_json(const std::string& jsonStr);
        static Alias getFromDto(const AliasDtoT& dto);
    };
    class NamespaceMetaDto : public NamespaceMeta {
    public:
        static NamespaceMeta from_json(const std::string& jsonStr);
        static NamespaceMeta getFromDto(const NamespaceMetaDtoT& dto);
    };
    class NamespaceDataDto :public NamespaceData {
    public:
        static NamespaceData from_json(const std::string& jsonStr);
        static NamespaceData getFromDto(const NamespaceDataDtoT& dto);
    };

    class NamespaceInfoDto : public NamespaceInfo {
    public:
        static NamespaceInfo from_json(const std::string& jsonStr);
        static NamespaceInfo getFromDto(const NamespaceInfoDtoT& dto);
    };
}