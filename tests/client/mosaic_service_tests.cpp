/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include <gtest/gtest.h>
#include "../config.h"

namespace xpx_chain_sdk::tests {

#define TEST_CLASS MosaicService

    auto client = xpx_chain_sdk::getClient(std::make_shared<xpx_chain_sdk::Config>(getTestConfiguration()));
    ClientData clientData;

    TEST(TEST_CLASS, getMosaicInfo) {
        auto accountInfo = client->account()->getAccountInfo(clientData.accountAddress);
        EXPECT_FALSE(accountInfo.mosaics.empty());

        auto mosaicId = accountInfo.mosaics[0].id;
        auto mosaicInfo = client->mosaics()->getMosaicInfo(mosaicId);

        EXPECT_EQ(mosaicId, mosaicInfo.data.mosaicId);
    }

    TEST(TEST_CLASS, getMosaicInfos) {
        auto accountInfo = client->account()->getAccountInfo(clientData.accountAddress);
        EXPECT_FALSE(accountInfo.mosaics.empty());

        std::vector<xpx_chain_sdk::MosaicId> mosaicIds;
        for (const xpx_chain_sdk::Mosaic& mosaic : accountInfo.mosaics) {
            mosaicIds.push_back(mosaic.id);
        }

        MultipleMosaicInfo multipleMosaicInfo = client->mosaics()->getMosaicInfos(mosaicIds);
        EXPECT_EQ(accountInfo.mosaics.size(), multipleMosaicInfo.mosaics.size());
    }

    TEST(TEST_CLASS, getMosaicsNames) {
        auto accountInfo = client->account()->getAccountInfo(clientData.accountAddress);
        EXPECT_FALSE(accountInfo.mosaics.empty());

        std::vector<xpx_chain_sdk::MosaicId> mosaicIds;
        for (const xpx_chain_sdk::Mosaic& mosaic : accountInfo.mosaics) {
            mosaicIds.push_back(mosaic.id);
        }

        MosaicNames mosaicNames = client->mosaics()->getMosaicsNames(mosaicIds);
        EXPECT_EQ(accountInfo.mosaics.size(), mosaicNames.names.size());
    }
}