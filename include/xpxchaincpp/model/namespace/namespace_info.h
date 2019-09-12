#pragma once

#include <stdint.h>
#include <string>

namespace xpx_chain_sdk {
    class Alias {
    public:
        uint64_t type;
        std::string mosaicId;
        std::string address;
    };

    class NamespaceMeta {
    public:
        bool active;
        uint64_t index;
        std::string id;
    };

    class NamespaceData {
    public:
        std::string owner;
        std::string ownerAddress;
        uint64_t startHeight;
        uint64_t endHeight;
        uint64_t depth;
        uint64_t level0;
        uint64_t level1;
        uint64_t level2;
        uint64_t type;
        Alias alias;
        uint64_t parentId;
    };

    class NamespaceInfo {
    public:
        NamespaceMeta meta;
        NamespaceData data;
    };
}