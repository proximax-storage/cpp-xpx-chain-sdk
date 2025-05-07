/**
*** Copyright 2022 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <string>
#include <vector>
#include "replicator.h"

namespace xpx_chain_sdk::replicators_page {

    class Pagination {
    public:
        uint64_t totalEntries;
        uint64_t pageNumber;
        uint64_t pageSize;
        uint64_t totalPages;
    };

    class ReplicatorsPage {
    public:
        MultipleReplicators data;
        Pagination pagination;
    };
}