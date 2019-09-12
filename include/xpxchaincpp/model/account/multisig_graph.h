/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <vector>
#include "multisig_info.h"


namespace xpx_chain_sdk {

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


