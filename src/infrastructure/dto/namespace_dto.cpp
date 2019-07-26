/*
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include "namespace_dto.h"
#include <infrastructure/json/parser.h>
#include <nemcpp/exceptions.h>

using namespace nem2_sdk;
using namespace nem2_sdk::internal::dto;
using nem2_sdk::internal::json::Parser;

Alias AliasDto::from_json(const std::string& jsonStr) {
    AliasDtoT dto;
    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

    Alias alias = AliasDto::getFromDto(dto);

    return alias;
}

Alias AliasDto::getFromDto(const AliasDtoT& dto) {

    Alias alias = {
            dto.value<"type"_>(),
            dto.value<"mosaicId"_>(),
            dto.value<"address"_>()
    };
    return alias;
}

NamespaceMeta NamespaceMetaDto::from_json(const std::string& jsonStr) {
    NamespaceMetaDtoT dto;
    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

    NamespaceMeta meta = NamespaceMetaDto::getFromDto(dto);

    return meta;
}

NamespaceMeta NamespaceMetaDto::getFromDto(const NamespaceMetaDtoT& dto) {
    NamespaceMeta namespaceMeta = {
            dto.value<"active"_>(),
            dto.value<"index"_>(),
            dto.value<"id"_>()
    };
    return namespaceMeta;
}

NamespaceData NamespaceDataDto::from_json(const std::string& jsonStr) {
    NamespaceDataDtoT dto;
    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

    NamespaceData data = NamespaceDataDto::getFromDto(dto);

    return data;
}

NamespaceData NamespaceDataDto::getFromDto(const NamespaceDataDtoT& dto) {
    NamespaceData namespaceData = {
            dto.value<"owner"_>(),
            dto.value<"ownerAddress"_>(),
            dto.value<"startHeight"_>(),
            dto.value<"endHeight"_>(),
            dto.value<"depth"_>(),
            dto.value<"level0"_>(),
            dto.value<"level1"_>(),
            dto.value<"level2"_>(),
            dto.value<"type"_>(),
            AliasDto::getFromDto(dto.value<"alias"_>()),
            dto.value<"parentId"_>()
    };
    return namespaceData;
}

NamespaceInfo NamespaceInfoDto::from_json(const std::string& jsonStr) {
    NamespaceInfoDtoT dto;
    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

    NamespaceInfo namespaceInfo = NamespaceInfoDto::getFromDto(dto);

    return namespaceInfo;
}

NamespaceInfo NamespaceInfoDto::getFromDto(const NamespaceInfoDtoT& dto) {
    NamespaceInfo namespaceInfo = {
            NamespaceMetaDto::getFromDto(dto.value<"meta"_>()),
            NamespaceDataDto::getFromDto(dto.value<"namespace"_>())
    };
    return namespaceInfo;
}