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

    TEST(TEST_CLASS, getNamespaceInfoById_zero) {
        EXPECT_THROW ({
            try {
                NamespaceId namespaceId = 0;
                NamespaceInfo namespaceInfo = client->namespaces()->getNamespaceInfoById(namespaceId);
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 404", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNamespaceInfoById_negative) {
        EXPECT_THROW ({
            try {
                NamespaceId namespaceId = -1;
                NamespaceInfo namespaceInfo = client->namespaces()->getNamespaceInfoById(namespaceId);
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 404", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }    

    TEST(TEST_CLASS, getNamespaceInfoById_max) {
        EXPECT_THROW ({
            try {
                NamespaceId namespaceId = UINT64_MAX;
                NamespaceInfo namespaceInfo = client->namespaces()->getNamespaceInfoById(namespaceId);
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 404", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNamespaceInfoByHexId) {
        // xpx namespace id
        NamespaceInfo namespaceInfo = client->namespaces()->getNamespaceInfoByHexId("BFFB42A19116BDF6");
        EXPECT_FALSE(namespaceInfo.meta.id.empty());
        EXPECT_TRUE(namespaceInfo.meta.index >= 0);
        EXPECT_FALSE(namespaceInfo.data.owner.empty());
        EXPECT_FALSE(namespaceInfo.data.ownerAddress.empty());
    }

    TEST(TEST_CLASS, getNamespaceInfoByHexId_longerLength) {
        EXPECT_THROW ({
            try {
                NamespaceInfo namespaceInfo = client->namespaces()->getNamespaceInfoByHexId("BFFB42A19116BDF6ABC"); //appended 'ABC'
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNamespaceInfoByHexId_shorterLength) {
        EXPECT_THROW ({
            try {
                NamespaceInfo namespaceInfo = client->namespaces()->getNamespaceInfoByHexId("BFFB42A19116B");   //removed 3 chars
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNamespaceInfoByHexId_empty) {
        EXPECT_THROW ({
            try {
                NamespaceInfo namespaceInfo = client->namespaces()->getNamespaceInfoByHexId("");
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNamespaceInfoByHexId_invalidHex) {
        EXPECT_THROW ({
            try {
                NamespaceInfo namespaceInfo = client->namespaces()->getNamespaceInfoByHexId("BFFB42A19116BDFZ");    //changed last char to 'z'
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNamespaceInfoByHexId_whitespace_trailing) {
        // xpx namespace id
        NamespaceInfo namespaceInfo = client->namespaces()->getNamespaceInfoByHexId("BFFB42A19116BDF6   ");
        EXPECT_FALSE(namespaceInfo.meta.id.empty());
        EXPECT_TRUE(namespaceInfo.meta.index >= 0);
        EXPECT_FALSE(namespaceInfo.data.owner.empty());
        EXPECT_FALSE(namespaceInfo.data.ownerAddress.empty());
    }
/*
    TEST(TEST_CLASS, getNamespaceInfoByHexId_whitespace_leading) {
        EXPECT_THROW ({
            try {
                NamespaceInfo namespaceInfo = client->namespaces()->getNamespaceInfoByHexId("   BFFB42A19116BDFZ");
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }
 */

    TEST(TEST_CLASS, getNamespaceInfoByAccount) {
        auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccount(clientData.publicKey);
        EXPECT_TRUE(multipleNamespaceInfo.namespaceInfos.empty());
    }

    TEST(TEST_CLASS, getNamespaceInfoByAccount_randomKey) {
        std::string key = "F30739A307E6EAA8C6D08ABCA168865DD4FE2104D1242C8F1960CA9B8DCAD260";
        auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccount(key);
        EXPECT_TRUE(multipleNamespaceInfo.namespaceInfos.empty());
    }

    TEST(TEST_CLASS, getNamespaceInfoByAccount_invalidKey) {
        EXPECT_THROW ({
            try {
                std::string key = "E92978122F00698856910664C480E8F3C2FDF0A733F42970FBD58A5145BD6F2Z"; //changed last char to z
                auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccount(key);
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNamespaceInfoByAccount_longerLength) {
        EXPECT_THROW ({
            try {
                std::string key = "E92978122F00698856910664C480E8F3C2FDF0A733F42970FBD58A5145BD6F21ABC"; //add 'ABC'
                auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccount(key);
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNamespaceInfoByAccount_shorterLength) {
        EXPECT_THROW ({
            try {
                std::string key = "E92978122F00698856910664C480E8F3C2FDF0A733F42970FBD58A5145BD6";  //remove last 3 char
                auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccount(key);
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNamespaceInfoByAccount_empty) {
        EXPECT_THROW ({
            try {
                std::string key = "";  
                auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccount(key);
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }
/*
    TEST(TEST_CLASS, getNamespaceInfoByAccount_tab_trailing) { //?
        std::string key = "E92978122F00698856910664C480E8F3C2FDF0A733F42970FBD58A5145BD6F21     "; 
        auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccount(key);
        EXPECT_TRUE(multipleNamespaceInfo.namespaceInfos.empty());
    }


    TEST(TEST_CLASS, getNamespaceInfoByAccount_whitespace_trailing) {
        EXPECT_THROW ({
            try {
                std::string key = "E92978122F00698856910664C480E8F3C2FDF0A733F42970FBD58A5145BD6F21    "; 
                auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccount(key);
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }
*/
    /*
    TEST(TEST_CLASS, getNamespaceInfoByAccount_tab_leading) {
        EXPECT_THROW ({
            try {
                std::string key = "    E92978122F00698856910664C480E8F3C2FDF0A733F42970FBD58A5145BD6F21"; 
                auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccount(key);
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }
    TEST(TEST_CLASS, getNamespaceInfoByAccount_whitespace_leading) {
        EXPECT_THROW ({
            try {
                std::string key = "     E92978122F00698856910664C480E8F3C2FDF0A733F42970FBD58A5145BD6F21"; 
                auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccount(key);
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }
    */

    TEST(TEST_CLASS, getNamespaceInfoByAccounts) {
        auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccounts({clientData.publicKey});
        EXPECT_TRUE(multipleNamespaceInfo.namespaceInfos.empty());
    }

   TEST(TEST_CLASS, getNamespaceInfoByAccounts_randomKey) {
        std::string key = "F30739A307E6EAA8C6D08ABCA168865DD4FE2104D1242C8F1960CA9B8DCAD260"; 
        auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccounts({key});
        EXPECT_TRUE(multipleNamespaceInfo.namespaceInfos.empty());
    }

    TEST(TEST_CLASS, getNamespaceInfoByAccounts_invalidKey) {
        EXPECT_THROW ({
            try {
                std::string key = "F30739A307E6EAA8C6D08ABCA168865DD4FE2104D1242C8F1960CA9B8DCAD26Z";   //changed last char to 'z'
                auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccounts({key});
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNamespaceInfoByAccounts_longerLength) {
        EXPECT_THROW ({
            try {
                std::string key = "F30739A307E6EAA8C6D08ABCA168865DD4FE2104D1242C8F1960CA9B8DCAD26ZABC";    //appended 'ABC'
                auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccounts({key});
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }
    TEST(TEST_CLASS, getNamespaceInfoByAccounts_shorterLength) {
        EXPECT_THROW ({
            try {
                std::string key = "F30739A307E6EAA8C6D08ABCA168865DD4FE2104D1242C8F1960CA9B8DCAD";  //removed last 3 char
                auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccounts({key});
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }


    TEST(TEST_CLASS, getNamespaceInfoByAccounts_empty) {
        auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccounts({});
        EXPECT_TRUE(multipleNamespaceInfo.namespaceInfos.empty());
    }

    TEST(TEST_CLASS, getNamespaceInfoByAccounts_emptyString) {
        EXPECT_THROW ({
            try {
                std::string key = ""; 
                auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccounts({key});
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNamespaceInfoByAccounts_multipleKeys) {
        std::string key = "E92978122F00698856910664C480E8F3C2FDF0A733F42970FBD58A5145BD6F21"; 
        auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccounts({key, key});
        EXPECT_TRUE(multipleNamespaceInfo.namespaceInfos.empty());
    }

    TEST(TEST_CLASS, getNamespaceInfoByAccounts_whitespace_leading) {
        EXPECT_THROW ({
            try {
                std::string key = "     F30739A307E6EAA8C6D08ABCA168865DD4FE2104D1242C8F1960CA9B8DCAD261"; 
                auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccounts({key});
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNamespaceInfoByAccounts_whitespace_trailing) {
        EXPECT_THROW ({
            try {
                std::string key = "F30739A307E6EAA8C6D08ABCA168865DD4FE2104D1242C8F1960CA9B8DCAD261    "; 
                auto multipleNamespaceInfo = client->namespaces()->getNamespaceInfoByAccounts({key});
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }

    TEST(TEST_CLASS, getNamespaceNames) {
        // xpx namespace id
        NamespaceNames names = client->namespaces()->getNamespaceNames({"BFFB42A19116BDF6"});
        EXPECT_FALSE(names.names.empty());
        EXPECT_EQ("prx.xpx", names.names[0].name);
    }

    TEST(TEST_CLASS, getNamespaceNames_invalidHex) {
        EXPECT_THROW ({
            try {
                NamespaceNames names = client->namespaces()->getNamespaceNames({"BFFB42A19116BDFZ"});
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }
    
    TEST(TEST_CLASS, getNamespaceNames_multipleIds) {
        // xpx namespace id
        NamespaceNames names = client->namespaces()->getNamespaceNames({"BFFB42A19116BDF6", "BFFB42A19116BDF6"});
        EXPECT_FALSE(names.names.empty());
        EXPECT_EQ("prx.xpx", names.names[0].name);
    }

    TEST(TEST_CLASS, getNamespaceNames_noIds) {
        NamespaceNames names = client->namespaces()->getNamespaceNames({});
        EXPECT_TRUE(names.names.empty());
    }

    TEST(TEST_CLASS, getNamespaceNames_emptyString) {
        EXPECT_THROW ({
            try {
                NamespaceNames names = client->namespaces()->getNamespaceNames({""});
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }


    TEST(TEST_CLASS, getNamespaceNames_whitespace_trailing) {
        EXPECT_THROW ({
            try {
                NamespaceNames names = client->namespaces()->getNamespaceNames({"BFFB42A19116BDF6   "});
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }


    TEST(TEST_CLASS, getNamespaceNames_whitespace_leading) {
        EXPECT_THROW ({
            try {
                NamespaceNames names = client->namespaces()->getNamespaceNames({"   BFFB42A19116BDF6"});
            }
            catch (xpx_chain_sdk::InvalidRequest& e) {
                EXPECT_STREQ("Server rejected the request. Error code: 409", e.what() );
                throw;
            }
        }, xpx_chain_sdk::InvalidRequest);
    }

}