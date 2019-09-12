/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <stdint.h>

namespace xpx_chain_sdk {
    class ScoreInfo {
    public:
        uint64_t scoreHigh;
        uint64_t scoreLow;
    };
}