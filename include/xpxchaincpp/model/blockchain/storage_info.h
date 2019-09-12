/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <stdint.h>

class StorageInfo {
public:
    uint64_t numBlocks;
    uint64_t numTransactions;
    uint64_t numAccounts;
};
