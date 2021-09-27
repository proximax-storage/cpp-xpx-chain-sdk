/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include <gtest/gtest.h>
#include "../config.h"
#include <string>

#include <boost/asio.hpp>
#include <chrono>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace xpx_chain_sdk::tests {

#define TEST_CLASS NotificationService

    boost::asio::io_context io;
    boost::asio::deadline_timer timer(io, boost::posix_time::seconds(60));

    ClientData clientData;
    auto client = xpx_chain_sdk::getClient(std::make_shared<xpx_chain_sdk::Config>(getTestConfiguration()));
    auto account = getTestAccount(clientData.privateKey);

    TEST(TEST_CLASS, addConfirmedAddedNotifier) {

        //create transaction
        Address recipient(clientData.publicKeyContainer);
        Mosaic mosaic(-4613020131619586570, 100);
        MosaicContainer mosaics{ mosaic };
        RawBuffer message("test message");

        //sign transaction
        std::unique_ptr<TransferTransaction> transferTransaction = CreateTransferTransaction(recipient, mosaics, message);
        account->signTransaction(transferTransaction.get());

        bool isReceived = false;
        std::mutex receivedMutex;
        std::unique_lock<std::mutex> lock(receivedMutex);

        //Handler
        ConfirmedAddedNotifier notifier = [&transferTransaction, &isReceived](const TransactionNotification& notification) {
            if (notification.meta.hash == ToHex(transferTransaction->hash())) {
                EXPECT_EQ(ToHex(transferTransaction->hash()), notification.meta.hash);

                auto transactionInfo = client->transactions()->getTransactionInfo(TransactionGroup::Confirmed, notification.meta.hash);
                EXPECT_EQ(TransactionType::Transfer,transactionInfo.get()->type);

                isReceived = true;
            }
        };

        client->notifications()->addConfirmedAddedNotifiers(recipient, {notifier});
        EXPECT_TRUE(client->transactions()->announceNewTransaction(transferTransaction->binary()));
       
        timer.async_wait([&isReceived, &recipient] ( ... ) {
            client->notifications()->removeConfirmedAddedNotifiers(recipient);
            EXPECT_TRUE(isReceived);
        });
    
        io.run();
    }

    TEST(TEST_CLASS, addUnconfirmedAddedNotifiers){
        //create transaction
        Address recipient(clientData.publicKeyContainer);
        Mosaic mosaic(-4613020131619586570, 100);
        MosaicContainer mosaics{ mosaic };
        RawBuffer message("test message");

        //sign transaction
        std::unique_ptr<TransferTransaction> transferTransaction = CreateTransferTransaction(recipient, mosaics, message);
        account->signTransaction(transferTransaction.get());

        bool isReceived = false;
        std::mutex receivedMutex;
        std::unique_lock<std::mutex> lock(receivedMutex);
        
        //Handler
        UnconfirmedAddedNotifier notifier = [&transferTransaction, &isReceived](const TransactionNotification& notification){
            if (notification.meta.hash == ToHex(transferTransaction->hash())) {
                EXPECT_EQ(ToHex(transferTransaction->hash()), notification.meta.hash);

                auto transactionInfo = client->transactions()->getTransactionInfo(TransactionGroup::Unconfirmed, notification.meta.hash);
                EXPECT_EQ(TransactionType::Transfer,transactionInfo.get()->type);

                isReceived = true;
            }
        };    

        client->notifications()->addUnconfirmedAddedNotifiers (recipient, {notifier});
        EXPECT_TRUE(client->transactions()->announceNewTransaction(transferTransaction->binary()));

        timer.async_wait([&isReceived, &recipient] ( ... ) {
            client->notifications()->removeUnconfirmedAddedNotifiers(recipient);
            EXPECT_TRUE(isReceived);
        });

        io.run();
    }

   TEST(TEST_CLASS, addUnconfirmedRemovedNotifiers){
        //create transaction
        Address recipient(clientData.publicKeyContainer);
        Mosaic mosaic(-4613020131619586570, 100);
        MosaicContainer mosaics{ mosaic };
        RawBuffer message("test message");

        //sign transaction
        std::unique_ptr<TransferTransaction> transferTransaction = CreateTransferTransaction(recipient, mosaics, message);
        account->signTransaction(transferTransaction.get());

        bool isReceived = false;
        std::mutex receivedMutex;
        std::unique_lock<std::mutex> lock(receivedMutex);
        
        //Handler
        UnconfirmedRemovedNotifier notifier = [&transferTransaction, &isReceived](const UnconfirmedRemovedTransactionNotification& transaction){
            if (transaction.meta.hash == ToHex(transferTransaction->hash())){
                EXPECT_EQ(ToHex(transferTransaction->hash()), transaction.meta.hash);

                auto transactionInfo = client->transactions()->getAnyTransactionInfo(transaction.meta.hash);
                EXPECT_EQ(TransactionType::Transfer,transactionInfo.get()->type);

                isReceived = true;
            }
        };
               
        client->notifications()->addUnconfirmedRemovedNotifiers(recipient, {notifier});
        EXPECT_TRUE(client->transactions()->announceNewTransaction(transferTransaction->binary()));

        timer.async_wait([&isReceived, &recipient] ( ... ) {
            client->notifications()->removeUnconfirmedRemovedNotifiers(recipient);
            EXPECT_TRUE(isReceived);
        });      

        io.run();
    }   
}