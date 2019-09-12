/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <string>
#include <vector>
#include "mosaic_name.h"

namespace xpx_chain_sdk {
    class MosaicNames {
    public:
        std::vector<MosaicName> names;
    };
}