#pragma once

#include <string>

namespace xpx_chain_sdk {
    class MosaicName {
    public:
        uint64_t parentId;
        uint64_t mosaicId;
        std::string name;
    };
}