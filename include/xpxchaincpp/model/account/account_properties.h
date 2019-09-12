/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include "account_property.h"
#include <vector>


namespace xpx_chain_sdk {
    class MultipleAccountProperty {
    public:
        std::vector<AccountProperties> accountProperties;
    };
}