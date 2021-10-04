/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include <gtest/gtest.h>
#include "../config.h"
#include <string>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <chrono>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace xpx_chain_sdk::tests {

#define TEST_CLASS NotificationService

    ClientData clientData;
    auto client = xpx_chain_sdk::getClient(std::make_shared<xpx_chain_sdk::Config>(getTestConfiguration()));
    auto account = getTestAccount(clientData.privateKey);

    void wait_Block(const boost::system::error_code&, int* count, bool* isReceived, boost::asio::deadline_timer* timer){
        if (*count < 120){
            ++(*count);
            if (*isReceived) {
                client->notifications()->removeBlockNotifiers();
            } else {
                timer->expires_from_now(boost::posix_time::seconds(1));
                timer->async_wait(boost::bind(wait_Block, boost::asio::placeholders::error, count, isReceived, timer));
            }
        }
    }

    TEST(TEST_CLASS, addBlockNotifiers){
        //Timer
        boost::asio::io_context io;
        boost::asio::deadline_timer timer(io, boost::posix_time::seconds(1));

        //Variables
        int count = 0;
        bool isReceived = false;
        
        //Handler
        BlockNotifier notifier = [&isReceived](const Block& block){
            isReceived = true;
        }; 

        client->notifications()->addBlockNotifiers ({notifier});

        timer.async_wait(boost::bind(wait_Block, boost::asio::placeholders::error, &count, &isReceived, &timer));

        io.run();
        EXPECT_TRUE(isReceived);
    }

    void wait_ConfirmedAdded(const boost::system::error_code&, int* count, bool* isReceived, Address recipient, boost::asio::deadline_timer* timer){
        if (*count < 120){
            ++(*count);
            if (*isReceived) {
                client->notifications()->removeConfirmedAddedNotifiers(recipient);
            } else {
                timer->expires_from_now(boost::posix_time::seconds(1));
                timer->async_wait(boost::bind(wait_ConfirmedAdded, boost::asio::placeholders::error, count, isReceived, recipient, timer));
            }
        }
    }
    
    TEST(TEST_CLASS, addConfirmedAddedNotifier) {
        //Timer
        boost::asio::io_context io;
        boost::asio::deadline_timer timer(io, boost::posix_time::seconds(1));

        //Create transaction
        Address recipient(clientData.publicKeyContainer);
        Mosaic mosaic(-4613020131619586570, 100);
        MosaicContainer mosaics{ mosaic };
        RawBuffer message("test message");

        //Sign transaction
        std::unique_ptr<TransferTransaction> transferTransaction = CreateTransferTransaction(recipient, mosaics, message);
        account->signTransaction(transferTransaction.get());

        //Variables
        int count = 0;
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
       
        timer.async_wait(boost::bind(wait_ConfirmedAdded, boost::asio::placeholders::error, &count, &isReceived, recipient, &timer));
    
        io.run();
        EXPECT_TRUE(isReceived);
    }

    void wait_UnconfirmedAdded(const boost::system::error_code&, int* count, bool* isReceived, Address recipient, boost::asio::deadline_timer* timer){
        if (*count < 120){
            ++(*count);
            if (*isReceived) {
                client->notifications()->removeUnconfirmedAddedNotifiers(recipient);
            } else {
                timer->expires_from_now(boost::posix_time::seconds(1));
                timer->async_wait(boost::bind(wait_UnconfirmedAdded, boost::asio::placeholders::error, count, isReceived, recipient, timer));
            }
        }
    }

    TEST(TEST_CLASS, addUnconfirmedAddedNotifiers){
        //Timer
        boost::asio::io_context io;
        boost::asio::deadline_timer timer(io, boost::posix_time::seconds(1));

        //Create transaction
        Address recipient(clientData.publicKeyContainer);
        Mosaic mosaic(-4613020131619586570, 100);
        MosaicContainer mosaics{ mosaic };
        RawBuffer message("test message");

        //Sign transaction
        std::unique_ptr<TransferTransaction> transferTransaction = CreateTransferTransaction(recipient, mosaics, message);
        account->signTransaction(transferTransaction.get());

        //Variables
        int count = 0;
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

        timer.async_wait(boost::bind(wait_UnconfirmedAdded, boost::asio::placeholders::error, &count, &isReceived, recipient, &timer));

        io.run();
        EXPECT_TRUE(isReceived);
    }

    void wait_UnconfirmedRemoved(const boost::system::error_code&, int* count, bool* isReceived, Address recipient, boost::asio::deadline_timer* timer){
        if (*count < 120){
            ++(*count);
            if (*isReceived) {
                client->notifications()->removeUnconfirmedRemovedNotifiers(recipient);
            } else {
                timer->expires_from_now(boost::posix_time::seconds(1));
                timer->async_wait(boost::bind(wait_UnconfirmedRemoved, boost::asio::placeholders::error, count, isReceived, recipient, timer));
            }
        }
    }

   TEST(TEST_CLASS, addUnconfirmedRemovedNotifiers){
        //Timer
        boost::asio::io_context io;
        boost::asio::deadline_timer timer(io, boost::posix_time::seconds(1));

        //Create transaction
        Address recipient(clientData.publicKeyContainer);
        Mosaic mosaic(-4613020131619586570, 100);
        MosaicContainer mosaics{ mosaic };
        RawBuffer message("test message");

        //Sign transaction
        std::unique_ptr<TransferTransaction> transferTransaction = CreateTransferTransaction(recipient, mosaics, message);
        account->signTransaction(transferTransaction.get());

        //Variables
        int count = 0;
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

        timer.async_wait(boost::bind(wait_UnconfirmedRemoved, boost::asio::placeholders::error, &count, &isReceived, recipient, &timer));  

        io.run();
        EXPECT_TRUE(isReceived);
    } 
}