#pragma once

#include <string>
#include <vector>
#include "block.h"

namespace xpx_chain_sdk {

    class MultipleBlock {
    public:
        std::vector<Block> blocks;
    };

}
