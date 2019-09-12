#pragma once

#include <stdint.h>

class StorageInfo {
public:
    uint64_t numBlocks;
    uint64_t numTransactions;
    uint64_t numAccounts;
};
