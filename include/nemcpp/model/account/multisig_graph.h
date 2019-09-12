#pragma once

#include <vector>
#include "multisig_info.h"


namespace xpx_sdk {

    class MultisigLevel {
    public:
        uint32_t level;
        std::vector<MultisigInfo> multisigEntries;
    };

    class MultisigGraph {
    public:
        std::vector<MultisigLevel> multisigLevels;
    };
}


