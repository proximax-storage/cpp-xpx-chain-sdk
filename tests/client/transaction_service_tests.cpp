/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include <gtest/gtest.h>
#include <condition_variable>
#include "../config.h"

namespace xpx_chain_sdk::tests {

#define TEST_CLASS TransactionService

    ClientData clientData;
    auto client = xpx_chain_sdk::getClient(std::make_shared<xpx_chain_sdk::Config>(getTestConfiguration()));

    auto account = [privateKey = clientData.privateKey]() {
        std::shared_ptr<xpx_chain_sdk::Account> account = std::make_shared<xpx_chain_sdk::Account>(
                [privateKey](xpx_chain_sdk::PrivateKeySupplierReason reason,
                              xpx_chain_sdk::PrivateKeySupplierParam param) {
                    xpx_chain_sdk::Key key;
                    xpx_chain_sdk::ParseHexStringIntoContainer(privateKey.c_str(),privateKey.size(), key);
                    return xpx_chain_sdk::PrivateKey(key.data(), key.size());
                });
        return account;
    }();

    auto announceNewTransaction = [](const std::vector<uint8_t>& binaryData, const std::string& hash) {
        try {
            EXPECT_TRUE(client->transactions()->announceNewTransaction(binaryData));

            std::cout << "announced new transaction: " << hash << std::endl;
        }
        catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    };

    auto onError = [](auto errorCode) {
        std::cout << "error code: " << errorCode << std::endl;
    };

    TEST(TEST_CLASS, getAnyTransactionInfo) {
        Address recipient(clientData.publicKeyContainer);
        // xpx id
        Mosaic mosaic(-4613020131619586570, 100);
        MosaicContainer mosaics{ mosaic };
        RawBuffer message("test message");

        std::unique_ptr<TransferTransaction> transferTransaction = CreateTransferTransaction(
                recipient, mosaics, message);

        account->signTransaction(transferTransaction.get());

        bool isReceived = false;
        std::mutex receivedMutex;
        std::unique_lock<std::mutex> lock(receivedMutex);
        std::condition_variable receivedCheck;

        Notifier<TransactionNotification> notifier([&transferTransaction, &receivedCheck, &isReceived, recipient](const notifierId& id, const TransactionNotification& notification) {
            if (notification.meta.hash == ToHex(transferTransaction->hash())) {
                EXPECT_EQ(ToHex(transferTransaction->hash()), notification.meta.hash);

                auto transactionInfo = client->transactions()->getAnyTransactionInfo(notification.meta.hash);
                EXPECT_EQ(TransactionType::Transfer, transactionInfo.get()->type);

                client->notifications()->removeConfirmedAddedNotifiers(recipient, [](){}, onError, { id });

                isReceived = true;
                receivedCheck.notify_all();
            }
        });

        auto onSuccess = [binaryData = transferTransaction->binary(), hash = ToHex(transferTransaction->hash())](){
            announceNewTransaction(binaryData, hash);
        };

        client->notifications()->addConfirmedAddedNotifiers(recipient, {notifier}, onSuccess, onError);

        receivedCheck.wait_for(lock, std::chrono::seconds(60), [&isReceived]() {
            return isReceived;
        });

        EXPECT_TRUE(isReceived);
    }

    TEST(TEST_CLASS, getTransactionInfo) {
        Address recipient(clientData.publicKeyContainer);
        // xpx id
        Mosaic mosaic(-4613020131619586570, 100);
        MosaicContainer mosaics{ mosaic };
        RawBuffer message("test message");

        std::unique_ptr<TransferTransaction> transferTransaction = CreateTransferTransaction(
                recipient, mosaics, message);

        account->signTransaction(transferTransaction.get());

        bool isReceived = false;
        std::mutex receivedMutex;
        std::unique_lock<std::mutex> lock(receivedMutex);
        std::condition_variable receivedCheck;

        Notifier<TransactionNotification> notifier([&transferTransaction, &receivedCheck, &isReceived, recipient](const notifierId& id, const TransactionNotification& notification) {
            if (notification.meta.hash == ToHex(transferTransaction->hash())) {
                EXPECT_EQ(ToHex(transferTransaction->hash()), notification.meta.hash);

                auto transactionInfo = client->transactions()->getTransactionInfo(TransactionGroup::Confirmed, notification.meta.hash);
                EXPECT_EQ(TransactionType::Transfer,transactionInfo.get()->type);

                client->notifications()->removeConfirmedAddedNotifiers(recipient, [](){}, onError, { id });

                isReceived = true;
                receivedCheck.notify_all();
            }
        });

        auto onSuccess = [binaryData = transferTransaction->binary(), hash = ToHex(transferTransaction->hash())](){
            announceNewTransaction(binaryData, hash);
        };

        client->notifications()->addConfirmedAddedNotifiers(recipient, {notifier}, onSuccess, onError);

        receivedCheck.wait_for(lock, std::chrono::seconds(60), [&isReceived]() {
            return isReceived;
        });

        EXPECT_TRUE(isReceived);
    }

    TEST(TEST_CLASS, getTransactionInfos) {
        Address recipient(clientData.publicKeyContainer);
        // xpx id
        Mosaic mosaic(-4613020131619586570, 100);
        MosaicContainer mosaics{ mosaic };
        RawBuffer message("test message");

        std::unique_ptr<TransferTransaction> transferTransaction = CreateTransferTransaction(
                recipient, mosaics, message);

        account->signTransaction(transferTransaction.get());

        bool isReceived = false;
        std::mutex receivedMutex;
        std::unique_lock<std::mutex> lock(receivedMutex);
        std::condition_variable receivedCheck;

        Notifier<TransactionNotification> notifier([&transferTransaction, &receivedCheck, &isReceived, recipient](const notifierId& id, const TransactionNotification& notification) {
            if (notification.meta.hash == ToHex(transferTransaction->hash())) {
                EXPECT_EQ(ToHex(transferTransaction->hash()), notification.meta.hash);

                auto transactionsInfos = client->transactions()->getTransactionInfos(TransactionGroup::Confirmed, {notification.meta.hash});
                EXPECT_EQ(1, transactionsInfos.transactions.size());

                client->notifications()->removeConfirmedAddedNotifiers(recipient, [](){}, onError, { id });

                isReceived = true;
                receivedCheck.notify_all();
            }
        });

        auto onSuccess = [binaryData = transferTransaction->binary(), hash = ToHex(transferTransaction->hash())](){
            announceNewTransaction(binaryData, hash);
        };

        client->notifications()->addConfirmedAddedNotifiers(recipient, {notifier}, onSuccess, onError);

        receivedCheck.wait_for(lock, std::chrono::seconds(60), [&isReceived]() {
            return isReceived;
        });

        EXPECT_TRUE(isReceived);
    }

    TEST(TEST_CLASS, getTransactionStatus) {
        Address recipient(clientData.publicKeyContainer);
        // xpx id
        Mosaic mosaic(-4613020131619586570, 100);
        MosaicContainer mosaics{ mosaic };
        RawBuffer message("test message");

        std::unique_ptr<TransferTransaction> transferTransaction = CreateTransferTransaction(
                recipient, mosaics, message);

        account->signTransaction(transferTransaction.get());

        bool isReceived = false;
        std::mutex receivedMutex;
        std::unique_lock<std::mutex> lock(receivedMutex);
        std::condition_variable receivedCheck;

        Notifier<TransactionNotification> notifier([&transferTransaction, &receivedCheck, &isReceived, recipient](const notifierId& id, const TransactionNotification& notification) {
            if (notification.meta.hash == ToHex(transferTransaction->hash())) {
                EXPECT_EQ(ToHex(transferTransaction->hash()), notification.meta.hash);

                auto transactionStatus = client->transactions()->getTransactionStatus(notification.meta.hash);
                EXPECT_EQ(notification.meta.hash,transactionStatus.hash);
                EXPECT_EQ(TransactionGroup::Confirmed,TransactionService::transactionGroupFromString(transactionStatus.group));

                client->notifications()->removeConfirmedAddedNotifiers(recipient, [](){}, onError, { id });

                isReceived = true;
                receivedCheck.notify_all();
            }
        });

        auto onSuccess = [binaryData = transferTransaction->binary(), hash = ToHex(transferTransaction->hash())](){
            announceNewTransaction(binaryData, hash);
        };

        client->notifications()->addConfirmedAddedNotifiers(recipient, {notifier}, onSuccess, onError);

        receivedCheck.wait_for(lock, std::chrono::seconds(60), [&isReceived]() {
            return isReceived;
        });

        EXPECT_TRUE(isReceived);
    }

    TEST(TEST_CLASS, getTransactionStatuses) {
        Address recipient(clientData.publicKeyContainer);
        // xpx id
        Mosaic mosaic(-4613020131619586570, 100);
        MosaicContainer mosaics{ mosaic };
        RawBuffer message("test message");

        std::unique_ptr<TransferTransaction> transferTransaction = CreateTransferTransaction(
                recipient, mosaics, message);

        account->signTransaction(transferTransaction.get());

        bool isReceived = false;
        std::mutex receivedMutex;
        std::unique_lock<std::mutex> lock(receivedMutex);
        std::condition_variable receivedCheck;

        Notifier<TransactionNotification> notifier([&transferTransaction, &receivedCheck, &isReceived, recipient](const notifierId& id, const TransactionNotification& notification) {
            if (notification.meta.hash == ToHex(transferTransaction->hash())) {
                EXPECT_EQ(ToHex(transferTransaction->hash()), notification.meta.hash);

                auto transactionStatuses = client->transactions()->getTransactionStatuses({notification.meta.hash});
                EXPECT_EQ(1, transactionStatuses.statuses.size());
                EXPECT_EQ(notification.meta.hash,transactionStatuses.statuses[0].hash);
                EXPECT_EQ(TransactionGroup::Confirmed,TransactionService::transactionGroupFromString(transactionStatuses.statuses[0].group));

                client->notifications()->removeConfirmedAddedNotifiers(recipient, [](){}, onError, { id });

                isReceived = true;
                receivedCheck.notify_all();
            }
        });

        auto onSuccess = [binaryData = transferTransaction->binary(), hash = ToHex(transferTransaction->hash())](){
            announceNewTransaction(binaryData, hash);
        };

        client->notifications()->addConfirmedAddedNotifiers(recipient, {notifier}, onSuccess, onError);

        receivedCheck.wait_for(lock, std::chrono::seconds(60), [&isReceived]() {
            return isReceived;
        });

        EXPECT_TRUE(isReceived);
    }

    TEST(TEST_CLASS, getTransactionsByGroupWithoutOptions) {
        transactions_page::TransactionsPage page = client->transactions()->getTransactionsByGroup(TransactionGroup::Confirmed);
        EXPECT_FALSE(page.transactions.empty());
        EXPECT_TRUE(page.pagination.totalPages > 0);
        EXPECT_TRUE(page.pagination.totalEntries > 0);
        EXPECT_TRUE(page.pagination.pageSize > 0);
        EXPECT_TRUE(page.pagination.pageNumber > 0);
    }

    TEST(TEST_CLASS, getTransactionsByGroupWithOptions) {
        PaginationOrderingOptions paginationOrderingOptions;
        paginationOrderingOptions.pageSize = 15;
        paginationOrderingOptions.pageNumber = 2;

        TransactionsPageOptions options;
        options.paginationOrderingOptions = paginationOrderingOptions;
        transactions_page::TransactionsPage page = client->transactions()->getTransactionsByGroup(TransactionGroup::Confirmed, options);
        EXPECT_FALSE(page.transactions.empty());
        EXPECT_TRUE(page.pagination.totalPages > 0);
        EXPECT_TRUE(page.pagination.totalEntries > 0);
        EXPECT_EQ(paginationOrderingOptions.pageSize.value(), page.pagination.pageSize);
        EXPECT_EQ(paginationOrderingOptions.pageNumber.value(), page.pagination.pageNumber);
    }

    TEST(TEST_CLASS, announceNewTransaction) {
        Address recipient(clientData.publicKeyContainer);
        // xpx id
        Mosaic mosaic(-4613020131619586570, 100);
        MosaicContainer mosaics{ mosaic };
        RawBuffer message("test message");

        std::unique_ptr<TransferTransaction> transferTransaction = CreateTransferTransaction(
                recipient, mosaics, message, Amount(1), NetworkDuration(20000), GetConfig().NetworkId);
        account->signTransaction(transferTransaction.get());
        EXPECT_TRUE(client->transactions()->announceNewTransaction(transferTransaction->binary()));
    }
}