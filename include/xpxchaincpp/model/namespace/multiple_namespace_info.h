#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include "namespace_info.h"

namespace xpx_chain_sdk {


    class MultipleNamespaceInfo {
    public:
        std::vector<NamespaceInfo> namespaceInfos;
    };
}