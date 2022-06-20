/**
*** Copyright 2022 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <string>
#include <vector>
#include "liquidity_provider.h"

namespace xpx_chain_sdk::liquidity_providers_page {

    class Pagination {
    public:
        uint64_t totalEntries;
        uint64_t pageNumber;
        uint64_t pageSize;
        uint64_t totalPages;
    };

    class LiquidityProvidersPage {
    public:
        MultipleLiquidityProviders data;
        Pagination pagination;
    };
}