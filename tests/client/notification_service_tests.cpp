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
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

typedef boost::asio::basic_waitable_timer< boost::asio::chrono::system_clock > system_timer;

namespace xpx_chain_sdk::tests {

void print(const boost::system::error_code& /*e*/)
{
  std::cout << "Hello, world!\n";
}

#define TEST_CLASS NotificationService

    boost::asio::io_context io;
    // boost::asio::steady_timer timer{iocontext};
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(60));

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
        std::condition_variable receivedCheck;
        // timer.expires_after(boost::asio::chrono::seconds(60));

        //Handler
        ConfirmedAddedNotifier notifier = [&transferTransaction, &receivedCheck, &isReceived, &lock](const TransactionNotification& notification) {
        
            if (notification.meta.hash == ToHex(transferTransaction->hash())) {
                EXPECT_EQ(ToHex(transferTransaction->hash()), notification.meta.hash);

                auto transactionInfo = client->transactions()->getTransactionInfo(TransactionGroup::Confirmed, notification.meta.hash);
                EXPECT_EQ(TransactionType::Transfer,transactionInfo.get()->type);

                isReceived = true;
                receivedCheck.notify_all();
            }

            t.async_wait(print);
            io.run();
            return isReceived;
        };

        client->notifications()->addConfirmedAddedNotifiers(recipient, {notifier});
        EXPECT_TRUE(client->transactions()->announceNewTransaction(transferTransaction->binary()));

        // receivedCheck.wait_for(lock, std::chrono::seconds(60), [&isReceived]() {
        //     return isReceived;
        // });
       
        client->notifications()->removeConfirmedAddedNotifiers(recipient);
        EXPECT_TRUE(isReceived);
    }

    // TEST(TEST_CLASS, addUnconfirmedAddedNotifiers){
    //     //create transaction
    //     Address recipient(clientData.publicKeyContainer);
    //     Mosaic mosaic(-4613020131619586570, 100);
    //     MosaicContainer mosaics{ mosaic };
    //     RawBuffer message("test message");

    //     //sign transaction
    //     auto account = getTestAccount(clientData.privateKey);
    //     std::unique_ptr<TransferTransaction> transferTransaction = CreateTransferTransaction(recipient, mosaics, message);
    //     account->signTransaction(transferTransaction.get());

    //     bool isReceived = false;
    //     std::mutex receivedMutex;
    //     std::unique_lock<std::mutex> lock(receivedMutex);
    //     std::condition_variable receivedCheck;
        
    //     //Handler, this one still copied from Transaction
    //     UnconfirmedAddedNotifier notifier = [&transferTransaction, &receivedCheck, &isReceived](const TransactionNotification& notification){
    //         if (notification.meta.hash == ToHex(transferTransaction->hash())) {
    //             EXPECT_EQ(ToHex(transferTransaction->hash()), notification.meta.hash);

    //             auto transactionInfo = client->transactions()->getAnyTransactionInfo(notification.meta.hash);
    //             EXPECT_EQ(TransactionType::Transfer,transactionInfo.get()->type);

    //             isReceived = true;
    //             receivedCheck.notify_all();
    //         }
    //     };    

    //     client->notifications()->addUnconfirmedAddedNotifiers (recipient, {notifier});
    //     EXPECT_TRUE(client->transactions()->announceNewTransaction(transferTransaction->binary()));

    //     receivedCheck.wait_for(lock, std::chrono::seconds(60), [&isReceived]() {
    //         return isReceived;
    //     });

    //     client->notifications()->removeUnconfirmedAddedNotifiers(recipient);
    //     EXPECT_TRUE(isReceived);
    // }

//    TEST(TEST_CLASS, addUnconfirmedRemovedNotifiers){
//         //create transaction
//         Address recipient(clientData.publicKeyContainer);
//         Mosaic mosaic(-4613020131619586570, 100);
//         MosaicContainer mosaics{ mosaic };
//         RawBuffer message("test message");

//         //sign transaction
//         auto account = getTestAccount(clientData.privateKey);
//         std::unique_ptr<TransferTransaction> transferTransaction = CreateTransferTransaction(recipient, mosaics, message);
//         account->signTransaction(transferTransaction.get());

//         bool isReceived = false;
//         std::mutex receivedMutex;
//         std::unique_lock<std::mutex> lock(receivedMutex);
//         std::condition_variable receivedCheck;
        
//         // Handler, this one still copied from Transaction
//         UnconfirmedRemovedNotifier notifier = [&transferTransaction, &receivedCheck, &isReceived](const transactions_info::TransactionInfo& info){
//             if (info.hash == ToHex(transferTransaction->hash())){
//                 EXPECT_EQ(ToHex(transferTransaction->hash()), info.hash);
//                 auto transactionInfo = client->transactions()->getAnyTransactionInfo(info.hash);

//                 EXPECT_EQ(TransactionType::Transfer,transactionInfo.get()->type);

//                 isReceived = true;
//                 receivedCheck.notify_all();
//             }
//         };
               
//         client->notifications()->addUnconfirmedRemovedNotifiers(recipient, {notifier});
//         EXPECT_TRUE(client->transactions()->announceNewTransaction(transferTransaction->binary()));

//         receivedCheck.wait_for(lock, std::chrono::seconds(60), [&isReceived]() {
//             return isReceived;
//         });

//         client->notifications()->removeUnconfirmedRemovedNotifiers(recipient);
//         EXPECT_TRUE(isReceived);
//     }
}