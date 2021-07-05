/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <string>

namespace xpx_chain_sdk {
    class MosaicName {
    public:
        uint64_t mosaicId;
        std::vector<std::string> names;
    };
}