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

        std::string accountAddress = "SD2L2LRSBZUMYV2T34C4UXOIAAWX4TWQSQGBPMQO";
        xpx_chain_sdk::Key publicKeyContainer;
        std::string publicKey = "0455F5326D5C380115E4BAE4B6EB2CEF4F83C3E5C4517494BFE11F5554135CF7";
        std::string privateKey = "819F72066B17FFD71B8B4142C5AEAE4B997B0882ABDF2C263B02869382BD93A0";
    };

    static xpx_chain_sdk::Config getTestConfiguration() {
        xpx_chain_sdk::Config config = xpx_chain_sdk::GetConfig();
        config.nodeAddress = "127.0.0.1";
        // config.nodeAddress = "18.141.208.138";
        // config.nodeAddress = "13.214.31.95";
        config.port = "3000";

        return config;
    }

    static std::shared_ptr<xpx_chain_sdk::Account> getTestAccount(const std::string &privateKey) {
        std::shared_ptr<xpx_chain_sdk::Account> account = std::make_shared<xpx_chain_sdk::Account>(
                [&privateKey](xpx_chain_sdk::PrivateKeySupplierReason reason,
                              xpx_chain_sdk::PrivateKeySupplierParam param) {
                    xpx_chain_sdk::Key key;
                    if (reason == xpx_chain_sdk::PrivateKeySupplierReason::Transaction_Signing ||
                        reason == xpx_chain_sdk::PrivateKeySupplierReason::Aggregate_Transaction_Cosigning) {
                        xpx_chain_sdk::ParseHexStringIntoContainer(privateKey.c_str(),
                                                                   privateKey.size(), key);
                    }
                    return xpx_chain_sdk::PrivateKey(key.data(), key.size());
                });
        return account;
    }
}
