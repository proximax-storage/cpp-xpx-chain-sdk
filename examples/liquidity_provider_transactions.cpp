/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/sdk.h>

#include <iostream>
#include <xpxchaincpp/model/message/message.h>
#include <memory>
#include <thread>

using namespace xpx_chain_sdk;


std::shared_ptr<xpx_chain_sdk::Account> CreateAccount(const std::string& privateKey) {
    return std::make_shared<xpx_chain_sdk::Account>([privateKey](xpx_chain_sdk::PrivateKeySupplierReason reason, xpx_chain_sdk::PrivateKeySupplierParam param) {
        xpx_chain_sdk::Key key;
        ParseHexStringIntoContainer(privateKey.c_str(), privateKey.size(), key);
        return xpx_chain_sdk::PrivateKey(key.data(), key.size());
    });
}

int main() {
	xpx_chain_sdk::Config config = xpx_chain_sdk::GetConfig();
	config.nodeAddress = "127.0.0.1";
	config.port = "3000";

	std::string accountAddress = "SDBSUK35HH4DHLLNOLZ66LCAIJ43I5BYSUAVBNSX";
	std::string publicKey = "E8D4B7BEB2A531ECA8CC7FD93F79A4C828C24BE33F99CF7C5609FF5CE14605F4";
	std::string privateKey = "2F985E4EC55D60C957C973BD1BEE2C0B3BA313A841D3EE4C74810805E6936053";

    std::string slashingAccountPublicKey = "E92978122F00698856910664C480E8F3C2FDF0A733F42970FBD58A5145BD6F21";
    std::string slashingAccountPrivateKey = "7AA907C3D80B3815BE4B4E1470DEEE8BB83BFEB330B9A82197603D09BA947230";

	auto client = xpx_chain_sdk::getClient(std::make_shared<xpx_chain_sdk::Config>(config));

    xpx_chain_sdk::MosaicId mosaicId = 2761073989369673764; // storage units
    Amount currencyDeposit = 100000;
    Amount initialMosaicsMinting = 100000;
    uint32_t slashingPeriod = 1;
    uint16_t windowSize = 10;
    Key slashingAccount;

    ParseHexStringIntoContainer(slashingAccountPublicKey.c_str(), slashingAccountPublicKey.size(), slashingAccount);

    uint32_t alpha = 500;
    uint32_t beta = 500;

    auto createLiquidityProviderTransaction = xpx_chain_sdk::CreateCreateLiquidityProviderTransaction(
            mosaicId, currencyDeposit, initialMosaicsMinting, slashingPeriod, windowSize, slashingAccount, alpha, beta);

    auto account = CreateAccount(privateKey);
    account->signTransaction(createLiquidityProviderTransaction.get());

    auto hash = ToHex(createLiquidityProviderTransaction->hash());
    std::cout <<  "transaction hash: " << hash << std::endl;

    std::atomic<bool> isFinished = false;

    Notifier<TransactionNotification> confirmedAddedNotifier([account, client, hash, &isFinished](const notifierId& id, const xpx_chain_sdk::TransactionNotification& notification) {
        if (notification.meta.hash == hash) {
            std::cout << __LINE__ << " : " << __FUNCTION__ << " : confirmedAddedNotifier thread id: " << std::this_thread::get_id() << "\n";
            std::cout << "confirmed transaction hash: " << hash << std::endl;

            client->notifications()->removeConfirmedAddedNotifiers(account->address(), [](){}, [](auto){}, {id});
            isFinished = true;
        } else {
            std::cout << "other confirmed transaction hash: " << notification.meta.hash << std::endl;
        }
    });

    Notifier<TransactionStatusNotification> statusNotifier([account, &isFinished](const notifierId& id, const xpx_chain_sdk::TransactionStatusNotification& notification) {
        std::cout <<  "transaction status notification is received : " << notification.status.c_str() << " : " << notification.hash.c_str() << std::endl;
        isFinished = true;
    });

    client->notifications()->addStatusNotifiers(account->address(), { statusNotifier }, [](){}, [](auto ){});

    auto announceTransaction = [client, hash, binaryData = createLiquidityProviderTransaction->binary()](){
        try {
            client->transactions()->announceNewTransaction(binaryData);

            std::cout << "announced new transaction: " << hash << std::endl;
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    };

    client->notifications()->addConfirmedAddedNotifiers(account->address(), { confirmedAddedNotifier }, announceTransaction, [](auto ){});

    while (!isFinished) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}