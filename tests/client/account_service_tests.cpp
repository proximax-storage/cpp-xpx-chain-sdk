/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include <gtest/gtest.h>
#include "../config.h"

namespace xpx_chain_sdk::tests {

#define TEST_CLASS AccountService

    auto client = xpx_chain_sdk::getClient(std::make_shared<xpx_chain_sdk::Config>(getTestConfiguration()));
    ClientData clientData;

    TEST(TEST_CLASS, getAccountInfo) {
        xpx_chain_sdk::AccountInfo ai = client->account()->getAccountInfo(clientData.accountAddress);
        EXPECT_EQ(ai.publicKey, clientData.publicKey);

        xpx_chain_sdk::AddressData addressData;
        xpx_chain_sdk::ParseHexStringIntoContainer(ai.address.c_str(), ai.address.size(), addressData);
        xpx_chain_sdk::Address address(addressData);

        EXPECT_EQ(address.encoded(), clientData.accountAddress);
    }

    TEST(TEST_CLASS, getAccountsInfo) {
        std::vector<std::string> addresses = { clientData.accountAddress };
        MultipleAccountInfo multipleAccountInfo = client->account()->getAccountsInfo(addresses);

        EXPECT_FALSE(multipleAccountInfo.accountInfos.empty());

        const int size = (int)multipleAccountInfo.accountInfos.size();
        for (int i = 0; i < size; i++) {
            xpx_chain_sdk::AddressData addressData;
            xpx_chain_sdk::ParseHexStringIntoContainer(
                    multipleAccountInfo.accountInfos[i].address.c_str(),
                    multipleAccountInfo.accountInfos[i].address.size(), addressData);
            xpx_chain_sdk::Address address(addressData);

            EXPECT_EQ(address.encoded(), addresses[i]);
        }
    }

    TEST(TEST_CLASS, getAccountProperties) {
        AccountProperties accountProperties = client->account()->getAccountProperties(clientData.publicKey);
        EXPECT_EQ(clientData.accountAddress, accountProperties.address);
        EXPECT_FALSE(accountProperties.properties.empty());
    }

    TEST(TEST_CLASS, getAccountsProperties) {
        std::vector<std::string> publicKeys = { clientData.publicKey };
        std::vector<std::string> addresses = { clientData.accountAddress };
        MultipleAccountProperty multipleAccountProperty = client->account()->getAccountsProperties(publicKeys);
        EXPECT_FALSE(multipleAccountProperty.accountProperties.empty());

        const int size = (int)multipleAccountProperty.accountProperties.size();
        for (int i = 0; i < size; i++) {
            AccountProperties accountProperties = multipleAccountProperty.accountProperties[i];
            EXPECT_EQ(addresses[i], accountProperties.address);
            EXPECT_FALSE(accountProperties.properties.empty());
        }
    }

    TEST(TEST_CLASS, getMultisigInfo) {
        MultisigInfo multisigInfo = client->account()->getMultisigInfo(clientData.accountAddress);
        EXPECT_EQ(clientData.accountAddress, multisigInfo.accountAddress);
        EXPECT_FALSE(multisigInfo.cosignatories.empty());
        EXPECT_FALSE(multisigInfo.multisigAccounts.empty());
    }

    TEST(TEST_CLASS, getMultisigAccountGraphInfo) {
        MultisigGraph multisigGraph = client->account()->getMultisigAccountGraphInfo(clientData.accountAddress);
        EXPECT_FALSE(multisigGraph.multisigLevels.empty());

        const int size = (int)multisigGraph.multisigLevels.size();
        for (int i = 0; i < size; i++) {
            MultisigLevel multisigLevel = multisigGraph.multisigLevels[i];
            EXPECT_FALSE(multisigLevel.multisigEntries.empty());

            const int multisigEntriesCount = (int)multisigLevel.multisigEntries.size();
            for (int i = 0; i < multisigEntriesCount; i++) {
                MultisigInfo multisigInfo = multisigLevel.multisigEntries[i];
                EXPECT_FALSE(multisigInfo.multisigAccounts.empty());
                EXPECT_FALSE(multisigInfo.cosignatories.empty());
            }
        }
    }

    TEST(TEST_CLASS, getAccountNames) {
        std::vector<std::string> addresses = { clientData.accountAddress };
        AccountNames accountNames = client->account()->getAccountNames(addresses);
        EXPECT_FALSE(accountNames.names.empty());

        const int size = (int)accountNames.names.size();
        for (int i = 0; i < size; i++) {
            xpx_chain_sdk::AddressData addressData;
            xpx_chain_sdk::ParseHexStringIntoContainer(
                    accountNames.names[i].address.c_str(),
                    accountNames.names[i].address.size(), addressData);
            xpx_chain_sdk::Address address(addressData);
            EXPECT_EQ(addresses[i], address.encoded());
        }
    }
}