#pragma once

#include <vector>
#include "mosaic.h"

namespace xpx_chain_sdk {
    class MultipleMosaicInfo {
    public:
        std::vector<MosaicInfo> mosaics;
    };
}