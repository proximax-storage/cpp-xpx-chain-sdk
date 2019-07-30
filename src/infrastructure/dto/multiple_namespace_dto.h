#include <nemcpp/utils/variadic_struct.h>
#include "namespace_dto.h"
#include <vector>
namespace nem2_sdk::internal::dto {

    using MultipleNamespaceDtoT = std::vector<NamespaceInfoDtoT>;

    class MultipleNamespaceDto {
    public:
        static std::vector<NamespaceInfo> from_json(const std::string& jsonStr);
        static std::vector<NamespaceInfo> getFromDto(const NamespaceInfoDtoT& dto);
    };
    static std::vector<NamespaceInfo> from_json(const std::string& jsonStr);
    static std::vector<NamespaceInfo> getFromDto(const NamespaceInfoDtoT& dto);
}






