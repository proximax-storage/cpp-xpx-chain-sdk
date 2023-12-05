/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#include <xpxchaincpp/sdk.h>

#include <iostream>
#include <xpxchaincpp/model/message/message.h>
#include <memory>

using namespace xpx_chain_sdk;

std::shared_ptr<xpx_chain_sdk::Account> CreateAccount(const std::string& privateKeyHex) {
    return std::make_shared<xpx_chain_sdk::Account>([privateKeyHex](xpx_chain_sdk::PrivateKeySupplierReason reason, xpx_chain_sdk::PrivateKeySupplierParam param) {
        xpx_chain_sdk::Key key;
        ParseHexStringIntoContainer(privateKeyHex.c_str(), privateKeyHex.size(), key);

        if (PrivateKeySupplierReason::Aggregate_Transaction_Cosigning == reason) {
            PrivateKey privateKey = xpx_chain_sdk::PrivateKey(key.data(), key.size());
            const KeyPair keyPair(std::move(privateKey));
            auto transaction = const_cast<Transaction *>(std::get<const Transaction *>(param));
            if (transaction) {
                std::cout << "Aggregate_Transaction_Cosigning hash: " << ToHex(transaction->hash()) << std::endl;
                SignTransaction(transaction, keyPair);
            }
        }


        return xpx_chain_sdk::PrivateKey(key.data(), key.size());
    });
}

int main () {
    xpx_chain_sdk::Config config = xpx_chain_sdk::GetConfig();
    config.nodeAddress = "127.0.0.1";
    config.port = "3000";

    std::string accountAddress = "SDHDCKRVH4NXSKIRX3M7PLPMP6F4O3Z3JUWKJHLB";
    std::string publicKey = "E92978122F00698856910664C480E8F3C2FDF0A733F42970FBD58A5145BD6F21";
    std::string privateKey = "7AA907C3D80B3815BE4B4E1470DEEE8BB83BFEB330B9A82197603D09BA947230";

    auto client = xpx_chain_sdk::getClient(config);
    auto sponsorAccount = CreateAccount(privateKey);

    Key senderKey;
    ParseHexStringIntoContainer(publicKey.c_str(), publicKey.size(), senderKey);
    Address sender(senderKey);

    Key recipientKey;
    std::string recipientKeyHex = "A384FBAAADBFF0405DDA0212D8A6C85F9164A08C24AFD15425927BCB274A45D4";
    ParseHexStringIntoContainer(recipientKeyHex.c_str(), recipientKeyHex.size(), recipientKey);
    Address recipient(recipientKey);

    // xpx id
    MosaicId xpx = 992621222383397347;
    Mosaic mosaic(xpx, 100);
    MosaicContainer mosaics{ mosaic };
    RawBuffer firstMessage("first transaction");

    Key signer;
    ParseHexStringIntoContainer(publicKey.c_str(), publicKey.size(), signer);

    std::unique_ptr<EmbeddedTransferTransaction> firstTransferTransaction = CreateEmbeddedTransferTransaction(recipient, mosaics, firstMessage, signer);

    RawBuffer secondMessage("second transaction");
    std::unique_ptr<EmbeddedTransferTransaction> secondTransferTransaction = CreateEmbeddedTransferTransaction(recipient, mosaics, secondMessage, signer);

    EmbeddedTransactions embeddedTransactions;
    embeddedTransactions.push_back(std::move(firstTransferTransaction));
    embeddedTransactions.push_back(std::move(secondTransferTransaction));

    auto aggregateBondedTransaction = CreateBondedAggregateTransaction(embeddedTransactions, {});
    sponsorAccount->signTransaction(aggregateBondedTransaction.get());

    auto lockFundsTransaction = CreateLockFundsTransaction(Mosaic(xpx, 10000000), 240, aggregateBondedTransaction->hash());
    sponsorAccount->signTransaction(lockFundsTransaction.get());

    std::cout << "lockHash: " << ToHex(lockFundsTransaction->lockHash()) << std::endl;
    std::cout << "main lock has: " << ToHex(lockFundsTransaction->hash()) << std::endl;

    Notifier<TransactionStatusNotification> statusNotifier([sponsorAccount](const notifierId& id, const xpx_chain_sdk::TransactionStatusNotification& notification) {
        std::cout <<  "transaction status notification is received : " << notification.status.c_str() << " : " << notification.hash.c_str() << std::endl;
    });

    Notifier<TransactionNotification> confirmedAddedNotifier([sponsorAccount,
                                                              client,
                                                              lockFundHash = ToHex(lockFundsTransaction->hash()),
                                                              binaryDataAggregate = aggregateBondedTransaction->binary(),
                                                              aggregateHash = ToHex(aggregateBondedTransaction->hash())](const notifierId& id, const xpx_chain_sdk::TransactionNotification& notification) {
        if (notification.meta.hash == lockFundHash) {
            std::cout << "confirmed lock fund transaction: " << lockFundHash << std::endl;

            client->transactions()->announceAggregateBoundedTransaction(binaryDataAggregate);
            std::cout << "announced new aggregateBoundedTransaction: " << aggregateHash << std::endl;
        } else if (notification.meta.hash == aggregateHash) {
            std::cout << "confirmed aggregate bonded transaction: " << aggregateHash << std::endl;
            client->notifications()->removeConfirmedAddedNotifiers(sponsorAccount->address(), [](){}, [](auto){}, {id});
        } else {
            std::cout << "other confirmed transaction hash: " << notification.meta.hash << std::endl;
        }
    });

    Notifier<std::shared_ptr<transactions_info::BasicTransaction>> parialAddedNotifier([sponsorAccount, client, hash = ToHex(aggregateBondedTransaction->hash())](const notifierId& id, std::shared_ptr<transactions_info::BasicTransaction> notification) {
        std::cout << "confirmed parialAddedNotifier: " << GetTransactionName(notification->type) << std::endl;
    });

    auto announceTransaction = [client,
                                binaryDataLockFund = lockFundsTransaction->binary(),
                                lockFundHash = ToHex(lockFundsTransaction->hash()),
                                binaryDataAggregate = aggregateBondedTransaction->binary(),
                                aggregateHash = ToHex(aggregateBondedTransaction->hash())](){
        try {
            //client->transactions()->announceNewTransaction(binaryDataLockFund);
            client->transactions()->announceAggregateBoundedTransaction(binaryDataAggregate);
            std::cout << "announced new aggregateBoundedTransaction: " << aggregateHash << std::endl;
            //std::cout << "announced new lockFundsTransaction: " << lockFundHash << std::endl;
        } catch (const xpx_chain_sdk::InvalidRequest& e) {
            std::cout << e.getErrorMessage().message << std::endl;
            std::cout << e.getErrorMessage().code << std::endl;
        }
    };

    auto onError = [](auto errorCode) {
        std::cout << "error code: " << errorCode << std::endl;
    };

    client->notifications()->addStatusNotifiers(sender, { statusNotifier }, [client, sender, confirmedAddedNotifier, parialAddedNotifier, announceTransaction, onError](){
        client->notifications()->addConfirmedAddedNotifiers(sender, { confirmedAddedNotifier }, [client, sender, parialAddedNotifier, announceTransaction, onError](){
            client->notifications()->addPartialAddedNotifiers(sender, { parialAddedNotifier }, announceTransaction, onError);
            }, onError);
        }, onError);

    char a;
    int ret = std::scanf("%c\n", &a);
    (void) ret; // ignore

    return 0;
}