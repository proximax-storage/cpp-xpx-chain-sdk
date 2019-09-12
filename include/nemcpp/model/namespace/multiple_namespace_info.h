#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include "namespace_info.h"

namespace xpx_sdk {


    class MultipleNamespaceInfo {
    public:
        std::vector<NamespaceInfo> namespaceInfos;
    };
}