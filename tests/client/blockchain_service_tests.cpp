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

    TEST(TEST_CLASS, getBlockchainHeight_checkPositive) {
        uint64_t blockchainHeight = client->blockchain()->getBlockchainHeight();
        EXPECT_TRUE(blockchainHeight > 0);
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

    TEST(TEST_CLASS, getBlockByHeight_positive) {
        uint64_t blockchainHeight = 1;
        Block block = client->blockchain()->getBlockByHeight(blockchainHeight);
        EXPECT_FALSE(block.data.blockTransactionsHash.empty());
        EXPECT_FALSE(block.data.stateHash.empty());
        EXPECT_TRUE(block.data.difficulty > 0);

    }

    TEST(TEST_CLASS, getBlockByHeight_zero) {
        EXPECT_THROW ({
            try {
                uint64_t blockchainHeight = 0;
                client->blockchain()->getBlockByHeight(blockchainHeight);
            } 
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 404", e.what() );
                throw;
            } 
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getBlockByHeight_negative) {
        EXPECT_THROW ({
            try {
                uint64_t blockchainHeight = -1;
                client->blockchain()->getBlockByHeight(blockchainHeight);
            } 
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            } 
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getBlockByHeight_max) {
        EXPECT_THROW ({
            try {
                uint64_t blockchainHeight = UINT64_MAX;
                client->blockchain()->getBlockByHeight(blockchainHeight);
            } 
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            } 
        }, xpx_chain_sdk::InvalidRequest);
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

    TEST(TEST_CLASS, getBlocksByHeightWithLimit_zero) {
        MultipleBlock multipleBlock = client->blockchain()->getBlocksByHeightWithLimit(0, 0);
        EXPECT_FALSE(multipleBlock.blocks.empty());

        for (const Block& block : multipleBlock.blocks) {
            EXPECT_FALSE(block.data.blockTransactionsHash.empty());
            EXPECT_FALSE(block.data.stateHash.empty());
            EXPECT_TRUE(block.data.difficulty > 0);
        }
    }

    TEST(TEST_CLASS, getBlocksByHeightWithLimit_zero_height) {
        MultipleBlock multipleBlock = client->blockchain()->getBlocksByHeightWithLimit(0, 30);
        EXPECT_FALSE(multipleBlock.blocks.empty());

        for (const Block& block : multipleBlock.blocks) {
            EXPECT_FALSE(block.data.blockTransactionsHash.empty());
            EXPECT_FALSE(block.data.stateHash.empty());
            EXPECT_TRUE(block.data.difficulty > 0);
        }
    }

    //TEST PASSED WITH LIMIT=0
    TEST(TEST_CLASS, getBlocksByHeightWithLimit_zero_limit) {
        MultipleBlock multipleBlock = client->blockchain()->getBlocksByHeightWithLimit(1, 0);
        EXPECT_FALSE(multipleBlock.blocks.empty());

        for (const Block& block : multipleBlock.blocks) {
            EXPECT_FALSE(block.data.blockTransactionsHash.empty());
            EXPECT_FALSE(block.data.stateHash.empty());
            EXPECT_TRUE(block.data.difficulty > 0);
        }
    }
    
    TEST(TEST_CLASS, getBlocksByHeightWithLimit_negative) {
        EXPECT_THROW ({
            try {
                client->blockchain()->getBlocksByHeightWithLimit(-1, -1);
            } 
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            } 
        }, xpx_chain_sdk::InvalidRequest);   
    }

    // EXCEPTION THROWN WITH NEGATIVE HEIGHT
    TEST(TEST_CLASS, getBlocksByHeightWithLimit_negative_height) {
        EXPECT_THROW ({
            try {
                client->blockchain()->getBlocksByHeightWithLimit(-1, 30);
            } 
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            } 
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getBlocksByHeightWithLimit_negative_limit) {
        EXPECT_THROW ({
            try {
                client->blockchain()->getBlocksByHeightWithLimit(1, -1);
            } 
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            } 
        }, xpx_chain_sdk::InvalidRequest);       
    }
    
    TEST(TEST_CLASS, getBlocksByHeightWithLimit_max) {
        EXPECT_THROW ({
            try {
                client->blockchain()->getBlocksByHeightWithLimit(UINT64_MAX, UINT64_MAX);
            } 
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            } 
        }, xpx_chain_sdk::InvalidRequest);   
    }

    TEST(TEST_CLASS, getBlocksByHeightWithLimitt_max_height) {
        EXPECT_THROW ({
            try {
                client->blockchain()->getBlocksByHeightWithLimit(UINT64_MAX, 30);
            } 
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            } 
        }, xpx_chain_sdk::InvalidRequest);   
    }

    TEST(TEST_CLASS, getBlocksByHeightWithLimit_max_limit) {
        EXPECT_THROW ({
            try {
                client->blockchain()->getBlocksByHeightWithLimit(1, UINT64_MAX);
            } 
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            } 
        }, xpx_chain_sdk::InvalidRequest);   
    }

    TEST(TEST_CLASS, getBlocksByHeightWithLimit_limit_25) {
        MultipleBlock multipleBlock = client->blockchain()->getBlocksByHeightWithLimit(1, 25);
        EXPECT_FALSE(multipleBlock.blocks.empty());

        for (const Block& block : multipleBlock.blocks) {
            EXPECT_FALSE(block.data.blockTransactionsHash.empty());
            EXPECT_FALSE(block.data.stateHash.empty());
            EXPECT_TRUE(block.data.difficulty > 0);
        }
    }

    TEST(TEST_CLASS, getBlocksByHeightWithLimit_limit_75) {
        MultipleBlock multipleBlock = client->blockchain()->getBlocksByHeightWithLimit(1, 75);
        EXPECT_FALSE(multipleBlock.blocks.empty());

        for (const Block& block : multipleBlock.blocks) {
            EXPECT_FALSE(block.data.blockTransactionsHash.empty());
            EXPECT_FALSE(block.data.stateHash.empty());
            EXPECT_TRUE(block.data.difficulty > 0);
        }
    }

    TEST(TEST_CLASS, getBlocksByHeightWithLimit_limit_101) {
        MultipleBlock multipleBlock = client->blockchain()->getBlocksByHeightWithLimit(1, 101);
        EXPECT_FALSE(multipleBlock.blocks.empty());

        for (const Block& block : multipleBlock.blocks) {
            EXPECT_FALSE(block.data.blockTransactionsHash.empty());
            EXPECT_FALSE(block.data.stateHash.empty());
            EXPECT_TRUE(block.data.difficulty > 0);
        }
    }

}

   