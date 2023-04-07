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

void sendTransferTransactionAsync(std::shared_ptr<IClient> client,
                                  std::shared_ptr<xpx_chain_sdk::Account> sponsorAccount,
                                  const MosaicContainer& mosaics,
                                  const std::string& receiverPublicKey,
                                  std::function<void()> onSuccess,
                                  std::function<void(const std::string& errorText)> onError) {
    std::string message = "message";
    xpx_chain_sdk::Key key;
    ParseHexStringIntoContainer(receiverPublicKey.c_str(), receiverPublicKey.size(), key);
    auto transferTransaction = xpx_chain_sdk::CreateTransferTransaction(xpx_chain_sdk::Address(key), mosaics, {message.data(), message.size()});

    sponsorAccount->signTransaction(transferTransaction.get());

    auto transferHash = ToHex(transferTransaction->hash());
    std::cout <<  "transferHash: " << transferHash << std::endl;

    Notifier<TransactionNotification> transferConfirmedAddedNotifier([sponsorAccount, client, transferHash, onSuccess](
            const notifierId& id, const xpx_chain_sdk::TransactionNotification& notification) {
        if (notification.meta.hash == transferHash) {
            std::cout << "confirmed transfer transaction hash: " << transferHash << std::endl;

            client->notifications()->removeConfirmedAddedNotifiers(sponsorAccount->address(), [](){}, [](auto){}, {id});
            onSuccess();
        } else {
            std::cout << "other confirmed transaction hash: " << notification.meta.hash << std::endl;
        }
    });

    Notifier<TransactionStatusNotification> transferStatusNotifier([sponsorAccount, transferHash, onError](const notifierId& id, const xpx_chain_sdk::TransactionStatusNotification& notification) {
        if (transferHash == notification.hash) {
            std::cout <<  "transfer transaction status notification is received : " << notification.status.c_str() << " : " << notification.hash.c_str() << std::endl;
            onError("ERROR. other confirmed transaction hash");
        } else {
            std::cout << "WARNING. transfer transaction status notification is received : " << notification.status.c_str() << " : " << notification.hash.c_str() << std::endl;
        }
    });

    client->notifications()->addStatusNotifiers(sponsorAccount->address(), { transferStatusNotifier }, [](){}, [](auto ){});

    auto announceTransferTransaction = [client, transferHash, binaryData = transferTransaction->binary(), onError](){
        try {
            client->transactions()->announceNewTransaction(binaryData);

            std::cout << "announced new transfer transaction: " << transferHash << std::endl;
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
            onError(e.what());
        }
    };

    client->notifications()->addConfirmedAddedNotifiers(sponsorAccount->address(), { transferConfirmedAddedNotifier }, announceTransferTransaction, [](auto ){});
}

int main() {
	xpx_chain_sdk::Config config = xpx_chain_sdk::GetConfig();
	config.nodeAddress = "127.0.0.1";
	config.port = "3000";

	std::string accountAddress = "SDBSUK35HH4DHLLNOLZ66LCAIJ43I5BYSUAVBNSX";
	std::string publicKey = "E8D4B7BEB2A531ECA8CC7FD93F79A4C828C24BE33F99CF7C5609FF5CE14605F4";
	std::string privateKey = "2F985E4EC55D60C957C973BD1BEE2C0B3BA313A841D3EE4C74810805E6936053";

    auto client = xpx_chain_sdk::getClient(std::make_shared<xpx_chain_sdk::Config>(config));

    std::string slashingAccountPublicKey = "E92978122F00698856910664C480E8F3C2FDF0A733F42970FBD58A5145BD6F21";
    std::string slashingAccountPrivateKey = "7AA907C3D80B3815BE4B4E1470DEEE8BB83BFEB330B9A82197603D09BA947230";

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

    const bool currencyBalanceIncrease = true;
    const Amount currencyBalanceChange = 100005;
    const bool mosaicBalanceIncrease = true;
    const Amount mosaicBalanceChange = 1370;

    auto manualRateChangeTransaction = xpx_chain_sdk::CreateManualRateChangeTransaction(
            mosaicId, currencyBalanceIncrease, currencyBalanceChange, mosaicBalanceIncrease, mosaicBalanceChange);

    account->signTransaction(manualRateChangeTransaction.get());

    auto manualRateChangeTransactionHash = ToHex(manualRateChangeTransaction->hash());
    std::cout << "manualRateChangeTransactionHash : " << manualRateChangeTransactionHash << std::endl;

    auto createManualRateChangeTransaction = [client, account, manualRateChangeTransactionHash, binaryData = manualRateChangeTransaction->binary()](){
        Notifier<TransactionNotification> confirmedAddedNotifier([account, client, manualRateChangeTransactionHash](
                const notifierId& id, const xpx_chain_sdk::TransactionNotification& notification) {
            if (notification.meta.hash == manualRateChangeTransactionHash) {
                std::cout << "confirmed manualRateChangeTransaction hash: " << manualRateChangeTransactionHash << std::endl;

                client->notifications()->removeConfirmedAddedNotifiers(account->address(), [](){}, [](auto){}, {id});
            } else {
                std::cout << "other confirmed transaction hash: " << notification.meta.hash << std::endl;
            }
        });

        Notifier<TransactionStatusNotification> statusNotifier([account](const notifierId& id, const xpx_chain_sdk::TransactionStatusNotification& notification) {
            std::cout <<  "manualRateChangeTransaction status notification is received : " << notification.status.c_str() << " : " << notification.hash.c_str() << std::endl;
        });

        client->notifications()->addStatusNotifiers(account->address(), { statusNotifier }, [](){}, [](auto ){});

        auto announceTransaction = [client, manualRateChangeTransactionHash, binaryData](){
            try {
                client->transactions()->announceNewTransaction(binaryData);

                std::cout << "announced new manualRateChangeTransaction: " << manualRateChangeTransactionHash << std::endl;
            } catch (std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        };

        client->notifications()->addConfirmedAddedNotifiers(account->address(), { confirmedAddedNotifier }, announceTransaction, [](auto ){});
    };

    std::atomic<bool> isFinished = false;

    auto createLpTransaction = [client, account, &hash, &isFinished, binaryData = createLiquidityProviderTransaction->binary()] () {
        Notifier<TransactionNotification> confirmedAddedNotifier([account, client, hash, &isFinished](const notifierId& id, const xpx_chain_sdk::TransactionNotification& notification) {
            if (notification.meta.hash == hash) {
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

        auto announceTransaction = [client, hash, binaryData](){
            try {
                client->transactions()->announceNewTransaction(binaryData);

                std::cout << "announced new transaction: " << hash << std::endl;
            } catch (std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        };

        client->notifications()->addConfirmedAddedNotifiers(account->address(), { confirmedAddedNotifier }, announceTransaction, [](auto ){});
    };

    auto sponsorAccount = CreateAccount("7AA907C3D80B3815BE4B4E1470DEEE8BB83BFEB330B9A82197603D09BA947230");
    // xpx id
    MosaicId xpx = 992621222383397347;
    Mosaic mosaic(xpx, 1000000);
    MosaicContainer mosaics{ mosaic };

    //createLpTransaction();
    //createManualRateChangeTransaction();

    sendTransferTransactionAsync(client, sponsorAccount, mosaics, publicKey, createLpTransaction, [](const std::string& errorText){
        std::cout << "error: " << errorText << std::endl;
    });



    while (!isFinished) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}