#include "multiple_namespace_dto.h"
#include <infrastructure/json/parser.h>

using namespace nem2_sdk;
using namespace nem2_sdk::internal::dto;

using nem2_sdk::internal::json::Parser;

std::vector<NamespaceInfo> MultipleNamespaceDto::from_json(const std::string& jsonStr) {
    MultipleNamespaceDtoT dto;
    auto result = Parser::Read(dto, jsonStr);

    if(!result) {
        NEM2_SDK_THROW_1(serialization_error, "Cannot parse JSON. Error with:", result.invalidField());
    }

    auto namespaceInfos = MultipleNamespaceDto::getFromDto(dto);
    return namespaceInfos;
}

std::vector<NamespaceInfo> MutlipleNamespaceDto::getFromDto(const NamespaceInfoDtoT& dto) {
    vector<NamespaceInfo> namespaceInfos;

    for(auto & niDtoT : dto) {
        namespaceInfos.push_back(NamespaceInfoDto::getFromDto(niDtoT));
    }
    return namespaceInfos;
}