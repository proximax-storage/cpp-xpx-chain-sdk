/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include <xpxchaincpp/sdk.h>

namespace xpx_chain_sdk::tests {

    struct ClientData {
        ClientData() {
            xpx_chain_sdk::ParseHexStringIntoContainer(
                    publicKey.c_str(),
                    publicKey.size(),
                    publicKeyContainer);
        }

        std::string accountAddress = "SAONE2UIW6DIH6BXKAW4OTF44XMJSQ23OUES6YBB";
        xpx_chain_sdk::Key publicKeyContainer;
        std::string publicKey = "8A578DC2A5F07B0ADE9DECCD626AE3C3610E72D7033191063C74E1DA9E9D4C09";
        std::string privateKey = "02F15E708EE15E834145048F0251B107A817542E6F288629141E44EF1A188FE8";
    };

    struct ClientData2 {
        ClientData2() {
            xpx_chain_sdk::ParseHexStringIntoContainer(
                    publicKey.c_str(),
                    publicKey.size(),
                    publicKeyContainer);
        }

        std::string accountAddress = "SD635GAAXIS6EHCEBLYDHJIRIHMMYAJMMMZH3YVC";
        xpx_chain_sdk::Key publicKeyContainer;
        std::string publicKey = "10AB82ACE20F2DC3EDC5E4C77B09420D60FB707B8D4AA86A768B05CF943A93EB";
        std::string privateKey = "3E86205091D90B661F95E986D7CC63D68FB11227E8AAF469A5612DB62F89606A";
    };

    static xpx_chain_sdk::Config getTestConfiguration() {
        xpx_chain_sdk::Config config = xpx_chain_sdk::GetConfig();
        config.nodeAddress = "127.0.0.1";
        config.port = "3000";

        return config;
    }

    static std::shared_ptr<xpx_chain_sdk::Account> getTestAccount(const std::string &privateKey) {
        std::shared_ptr<xpx_chain_sdk::Account> account = std::make_shared<xpx_chain_sdk::Account>(
                [&privateKey](xpx_chain_sdk::PrivateKeySupplierReason reason,
                              xpx_chain_sdk::PrivateKeySupplierParam param) {
                    xpx_chain_sdk::Key key;
                    if (reason == xpx_chain_sdk::PrivateKeySupplierReason::Transaction_Signing) {
                        xpx_chain_sdk::ParseHexStringIntoContainer(privateKey.c_str(),
                                                                   privateKey.size(), key);
                    }

                    return xpx_chain_sdk::PrivateKey(key.data(), key.size());
                });

        return account;
    }
}
