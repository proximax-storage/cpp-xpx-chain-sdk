/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <string>
#include <vector>
#include <xpxchaincpp/model/mosaic/mosaic.h>


namespace xpx_chain_sdk {
    class AccountName {
    public:
        std::string address;
        std::vector<std::string> names;
    };

    class AccountNames {
    public:
        std::vector<AccountName> names;
    };
}