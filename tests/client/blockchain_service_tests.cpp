/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include <gtest/gtest.h>
#include "../config.h"

namespace xpx_chain_sdk::tests {

#define TEST_CLASS BlockchainService

    auto client = xpx_chain_sdk::getClient(std::make_shared<xpx_chain_sdk::Config>(getTestConfiguration()));

    TEST(TEST_CLASS, getBlockchainHeight) {
        uint64_t blockchainHeight = client->blockchain()->getBlockchainHeight();
        EXPECT_TRUE(blockchainHeight);
    }

    TEST(TEST_CLASS, getCurrentScore) {
        ScoreInfo scoreInfo = client->blockchain()->getCurrentScore();
        EXPECT_TRUE(scoreInfo.scoreHigh >= 0);
        EXPECT_TRUE(scoreInfo.scoreLow >= 0);
    }

    TEST(TEST_CLASS, getStorageInfo) {
        StorageInfo storageInfo = client->blockchain()->getStorageInfo();
        EXPECT_TRUE(storageInfo.numAccounts >= 0);
        EXPECT_TRUE(storageInfo.numBlocks >= 0);
        EXPECT_TRUE(storageInfo.numTransactions > 0);
    }

    TEST(TEST_CLASS, getBlockByHeight) {
        uint64_t blockchainHeight = client->blockchain()->getBlockchainHeight();
        Block block = client->blockchain()->getBlockByHeight(blockchainHeight);
        EXPECT_FALSE(block.data.blockTransactionsHash.empty());
        EXPECT_FALSE(block.data.stateHash.empty());
        EXPECT_TRUE(block.data.difficulty > 0);
    }

    TEST(TEST_CLASS, getBlocksByHeightWithLimit) {
        MultipleBlock multipleBlock = client->blockchain()->getBlocksByHeightWithLimit(1, 30);
        EXPECT_FALSE(multipleBlock.blocks.empty());

        for (const Block& block : multipleBlock.blocks) {
            EXPECT_FALSE(block.data.blockTransactionsHash.empty());
            EXPECT_FALSE(block.data.stateHash.empty());
            EXPECT_TRUE(block.data.difficulty > 0);
        }
    }
}