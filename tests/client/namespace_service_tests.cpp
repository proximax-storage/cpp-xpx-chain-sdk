/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include <gtest/gtest.h>
#include "../config.h"

namespace xpx_chain_sdk::tests {

#define TEST_CLASS NamespaceService

    auto client = xpx_chain_sdk::getClient(std::make_shared<xpx_chain_sdk::Config>(getTestConfiguration()));
    ClientData clientData;

    TEST(TEST_CLASS, getNamespaceInfoById) {
        // xpx namespace id
        NamespaceId namespaceId = -4613020131619586570;
        NamespaceInfo namespaceInfo = client->namespaces()->getNamespaceInfoById(namespaceId);
        EXPECT_FALSE(namespaceInfo.meta.id.empty());
        EXPECT_TRUE(namespaceInfo.meta.index >= 0);
        EXPECT_FALSE(namespaceInfo.data.owner.empty());
        EXPECT_FALSE(namespaceInfo.data.ownerAddress.empty());
    }

    TEST(TEST_CLASS, getNamespaceInfoByHexId) {
        // xpx namespace id
        NamespaceInfo namespaceInfo = client->namespaces()->getNamespaceInfoByHexId("BFFB42A19116BDF6");
        EXPECT_FALSE(namespaceInfo.meta.id.empty());
        EXPECT_TRUE(namespaceInfo.meta.index >= 0);
        EXPECT_FALSE(namespaceInfo.data.owner.empty());
        EXPECT_FALSE(namespaceInfo.data.ownerAddress.empty());
    }

    TEST(TEST_CLASS, getNamespaceInfoByAccount) {
        auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccount(clientData.publicKey);
        EXPECT_TRUE(multipleNamespaceInfo.namespaceInfos.empty());
    }

    TEST(TEST_CLASS, getNamespaceInfoByAccounts) {
        auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccounts({clientData.publicKey});
        EXPECT_TRUE(multipleNamespaceInfo.namespaceInfos.empty());
    }

    TEST(TEST_CLASS, getNamespaceNames) {
        // xpx namespace id
        NamespaceNames names = client->namespaces()->getNamespaceNames({"BFFB42A19116BDF6"});
        EXPECT_FALSE(names.names.empty());
        EXPECT_EQ("prx.xpx", names.names[0].name);
    }
}