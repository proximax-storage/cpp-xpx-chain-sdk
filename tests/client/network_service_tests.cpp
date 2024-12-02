/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include <gtest/gtest.h>
#include "../config.h"

namespace xpx_chain_sdk::tests {

#define TEST_CLASS NetworkService

    auto client = xpx_chain_sdk::getClient(std::make_shared<xpx_chain_sdk::Config>(getTestConfiguration()));

    TEST(TEST_CLASS, getNetworkConfig) {
        NetworkConfig networkConfig = client->network()->getNetworkConfig();
        EXPECT_TRUE(networkConfig.data.height > 0);
        EXPECT_FALSE(networkConfig.data.supportedEntityVersions.empty());
        EXPECT_FALSE(networkConfig.data.config.empty());
    }

    TEST(TEST_CLASS, getNetworkConfigAtHeight) {
        NetworkConfig networkConfig = client->network()->getNetworkConfigAtHeight(1);
        EXPECT_TRUE(networkConfig.data.height > 0);
        EXPECT_FALSE(networkConfig.data.supportedEntityVersions.empty());
        EXPECT_FALSE(networkConfig.data.config.empty());
    }
    TEST(TEST_CLASS, getNetworkConfigAtHeight_zero) {
        EXPECT_THROW ({
            try {
                NetworkConfig networkConfig = client->network()->getNetworkConfigAtHeight(0);
            } 
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 404", e.what() );
                throw;
            } 
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNetworkConfigAtHeight_negative) {
        EXPECT_THROW ({
            try {
                NetworkConfig networkConfig = client->network()->getNetworkConfigAtHeight(-1);
            } 
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            } 
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNetworkConfigAtHeight_max) {
        EXPECT_THROW ({
            try {
                uint64_t blockchainHeight = UINT64_MAX;
                NetworkConfig networkConfig = client->network()->getNetworkConfigAtHeight(blockchainHeight);
            } 
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            } 
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNetworkInfo) {
        NetworkInfo networkInfo = client->network()->getNetworkInfo();
        EXPECT_FALSE(networkInfo.description.empty());
        EXPECT_FALSE(networkInfo.name.empty());
    }

    TEST(TEST_CLASS, getNetworkVersion) {
        NetworkVersion networkVersion = client->network()->getNetworkVersion();
        EXPECT_TRUE(networkVersion.data.height > 0);
        EXPECT_TRUE(networkVersion.data.version > 0);
    }

    TEST(TEST_CLASS, getNetworkVersionAtHeight) {
        NetworkVersion networkVersion = client->network()->getNetworkVersionAtHeight(1);
        EXPECT_TRUE(networkVersion.data.height > 0);
        EXPECT_TRUE(networkVersion.data.version > 0);
    }

    TEST(TEST_CLASS, getNetworkVersionAtHeight_zero) {
        EXPECT_THROW ({
            try {
                NetworkVersion networkVersion = client->network()->getNetworkVersionAtHeight(0);
            } 
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 404", e.what() );
                throw;
            } 
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNetworkVersionAtHeight_negative) {
        EXPECT_THROW ({
            try {
                NetworkVersion networkVersion = client->network()->getNetworkVersionAtHeight(-1);
            } 
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            } 
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNetworkVersionAtHeight_max) {
        EXPECT_THROW ({
            try {
                uint64_t blockchainHeight = UINT64_MAX;
                NetworkVersion networkVersion = client->network()->getNetworkVersionAtHeight(blockchainHeight);
            } 
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            } 
        }, xpx_chain_sdk::InvalidRequest);
    }
}