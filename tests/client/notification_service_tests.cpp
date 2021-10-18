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

//     void waitBlock(int currentIteration, const int maxIterations, const bool& isReceived, boost::asio::deadline_timer& timer){
//         if (currentIteration < maxIterations){
//             ++currentIteration;
//             if (isReceived) {
//                 client->notifications()->removeBlockNotifiers();
//             } else {
//                 timer.expires_from_now(boost::posix_time::seconds(1));
//                 timer.async_wait([currentIteration, maxIterations, &isReceived, &timer](const boost::system::error_code& errorCode) {
//                     EXPECT_FALSE(errorCode.value());
//                     waitBlock(currentIteration, maxIterations, isReceived, timer);
//                 });
//             }
//         }
//     }

//     TEST(TEST_CLASS, addBlockNotifiers){
//         //Timer
//         boost::asio::io_context io;
//         boost::asio::deadline_timer timer(io, boost::posix_time::seconds(1));

//         //Variables
//         const int maxIterations = 15;
//         int currentIteration = 0;
//         bool isReceived = false;
        
//         //Handler
//         BlockNotifier notifier = [&isReceived](const Block& block){
//             isReceived = true;
//         }; 

//         client->notifications()->addBlockNotifiers ({notifier});

//         timer.async_wait([currentIteration, &isReceived, &timer](const boost::system::error_code& errorCode) {
//             EXPECT_FALSE(errorCode.value());
//             waitBlock(currentIteration, maxIterations, isReceived, timer);
//         });

//         io.run();
//         EXPECT_TRUE(isReceived);
//     }

//     void waitConfirmedAdded(int currentIteration, const int maxIterations, const bool& isReceived, const Address& recipient, boost::asio::deadline_timer& timer){
//         if (currentIteration < maxIterations){
//             ++currentIteration;
//             if (isReceived) {
//                 client->notifications()->removeConfirmedAddedNotifiers(recipient);
//             } else {
//                 timer.expires_from_now(boost::posix_time::seconds(1));
//                 timer.async_wait([currentIteration, maxIterations, &isReceived, &recipient, &timer](const boost::system::error_code& errorCode) {
//                     EXPECT_FALSE(errorCode.value());
//                     waitConfirmedAdded(currentIteration, maxIterations, isReceived, recipient, timer);
//                 });
//             }
//         }
//     }
    
//     TEST(TEST_CLASS, addConfirmedAddedNotifier) {
//         //Timer
//         boost::asio::io_context io;
//         boost::asio::deadline_timer timer(io, boost::posix_time::seconds(1));

//         //Create transaction
//         Address recipient(clientData.publicKeyContainer);
//         Mosaic mosaic(-4613020131619586570, 100);
//         MosaicContainer mosaics{ mosaic };
//         RawBuffer message("test message");

//         //Sign transaction
//         std::unique_ptr<TransferTransaction> transferTransaction = CreateTransferTransaction(recipient, mosaics, message);
//         account->signTransaction(transferTransaction.get());

//         //Variables
//         const int maxIterations = 15;
//         int currentIteration = 0;
//         bool isReceived = false;

//         //Handler
//         ConfirmedAddedNotifier notifier = [&transferTransaction, &isReceived](const TransactionNotification& notification) {
//             if (notification.meta.hash == ToHex(transferTransaction->hash())) {
//                 EXPECT_EQ(ToHex(transferTransaction->hash()), notification.meta.hash);

//                 auto transactionInfo = client->transactions()->getTransactionInfo(TransactionGroup::Confirmed, notification.meta.hash);
//                 EXPECT_EQ(TransactionType::Transfer,transactionInfo.get()->type);

//                 isReceived = true;
//             }
//         };

//         client->notifications()->addConfirmedAddedNotifiers(recipient, {notifier});
//         EXPECT_TRUE(client->transactions()->announceNewTransaction(transferTransaction->binary()));

//         timer.async_wait([currentIteration, &isReceived, &recipient, &timer](const boost::system::error_code& errorCode) {
//             EXPECT_FALSE(errorCode.value());
//             waitConfirmedAdded(currentIteration, maxIterations, isReceived, recipient, timer);
//         });
    
//         io.run();
//         EXPECT_TRUE(isReceived);
//     }

//     void waitUnconfirmedAdded(int currentIteration, const int maxIterations, const bool& isReceived, const Address& recipient, boost::asio::deadline_timer& timer){
//         if (currentIteration < maxIterations){
//             ++currentIteration;
//             if (isReceived) {
//                 client->notifications()->removeUnconfirmedAddedNotifiers(recipient);
//             } else {
//                 timer.expires_from_now(boost::posix_time::seconds(1));
//                 timer.async_wait([currentIteration, maxIterations, &isReceived, &recipient, &timer](const boost::system::error_code& errorCode) {
//                     EXPECT_FALSE(errorCode.value());
//                     waitUnconfirmedAdded(currentIteration, maxIterations, isReceived, recipient, timer);
//                 });
//             }
//         }
//     }

//     TEST(TEST_CLASS, addUnconfirmedAddedNotifiers){
//         //Timer
//         boost::asio::io_context io;
//         boost::asio::deadline_timer timer(io, boost::posix_time::seconds(1));

//         //Create transaction
//         Address recipient(clientData.publicKeyContainer);
//         Mosaic mosaic(-4613020131619586570, 100);
//         MosaicContainer mosaics{ mosaic };
//         RawBuffer message("test message");

//         //Sign transaction
//         std::unique_ptr<TransferTransaction> transferTransaction = CreateTransferTransaction(recipient, mosaics, message);
//         account->signTransaction(transferTransaction.get());

//         //Variables
//         const int maxIterations = 15;
//         int currentIteration = 0;
//         bool isReceived = false;
        
//         //Handler
//         UnconfirmedAddedNotifier notifier = [&transferTransaction, &isReceived](const TransactionNotification& notification){
//             if (notification.meta.hash == ToHex(transferTransaction->hash())) {
//                 EXPECT_EQ(ToHex(transferTransaction->hash()), notification.meta.hash);

//                 auto transactionInfo = client->transactions()->getTransactionInfo(TransactionGroup::Unconfirmed, notification.meta.hash);
//                 EXPECT_EQ(TransactionType::Transfer,transactionInfo.get()->type);

//                 isReceived = true;
//             }
//         };    

//         client->notifications()->addUnconfirmedAddedNotifiers (recipient, {notifier});
//         EXPECT_TRUE(client->transactions()->announceNewTransaction(transferTransaction->binary()));

//         timer.async_wait([currentIteration, &isReceived, &recipient, &timer](const boost::system::error_code& errorCode) {
//             EXPECT_FALSE(errorCode.value());
//             waitUnconfirmedAdded(currentIteration, maxIterations, isReceived, recipient, timer);
//         });
//         io.run();
//         EXPECT_TRUE(isReceived);
//     }

//     void waitUnconfirmedRemoved(int currentIteration, const int maxIterations, const bool& isReceived, const Address& recipient, boost::asio::deadline_timer& timer){
//         if (currentIteration < maxIterations){
//             ++currentIteration;
//             if (isReceived) {
//                 client->notifications()->removeUnconfirmedRemovedNotifiers(recipient);
//             } else {
//                 timer.expires_from_now(boost::posix_time::seconds(1));
//                 timer.async_wait([currentIteration, maxIterations, &isReceived, &recipient, &timer](const boost::system::error_code& errorCode) {
//                     EXPECT_FALSE(errorCode.value());
//                     waitUnconfirmedRemoved(currentIteration, maxIterations, isReceived, recipient, timer);
//                 });
//             }
//         }
//     }

//    TEST(TEST_CLASS, addUnconfirmedRemovedNotifiers){
//         //Timer
//         boost::asio::io_context io;
//         boost::asio::deadline_timer timer(io, boost::posix_time::seconds(1));

//         //Create transaction
//         Address recipient(clientData.publicKeyContainer);
//         Mosaic mosaic(-4613020131619586570, 100);
//         MosaicContainer mosaics{ mosaic };
//         RawBuffer message("test message");

//         //Sign transaction
//         std::unique_ptr<TransferTransaction> transferTransaction = CreateTransferTransaction(recipient, mosaics, message);
//         account->signTransaction(transferTransaction.get());

//         //Variables
//         const int maxIterations = 15;
//         int currentIteration = 0;
//         bool isReceived = false;
        
//         //Handler
//         UnconfirmedRemovedNotifier notifier = [&transferTransaction, &isReceived](const UnconfirmedRemovedTransactionNotification& transaction){
//             if (transaction.meta.hash == ToHex(transferTransaction->hash())){
//                 EXPECT_EQ(ToHex(transferTransaction->hash()), transaction.meta.hash);

//                 auto transactionInfo = client->transactions()->getAnyTransactionInfo(transaction.meta.hash);
//                 EXPECT_EQ(TransactionType::Transfer,transactionInfo.get()->type);

//                 isReceived = true;
//             }
//         };
               
//         client->notifications()->addUnconfirmedRemovedNotifiers(recipient, {notifier});
//         EXPECT_TRUE(client->transactions()->announceNewTransaction(transferTransaction->binary()));

//          timer.async_wait([currentIteration, &isReceived, &recipient, &timer](const boost::system::error_code& errorCode) {
//             EXPECT_FALSE(errorCode.value());
//             waitUnconfirmedRemoved(currentIteration, maxIterations, isReceived, recipient, timer);
//         });  

//         io.run();
//         EXPECT_TRUE(isReceived);
//     }  

    void waitPartialAdded(int currentIteration, const int maxIterations, const bool& isReceived, Address account, boost::asio::deadline_timer& timer){
        if (currentIteration < maxIterations){
            ++currentIteration;
            if (isReceived) {
                client->notifications()->removePartialAddedNotifiers(account);
            } else {
                timer.expires_from_now(boost::posix_time::seconds(1));
                timer.async_wait([currentIteration, maxIterations, &isReceived, account ,&timer](const boost::system::error_code& errorCode) {
                    EXPECT_FALSE(errorCode.value());
                    waitPartialAdded(currentIteration, maxIterations, isReceived, account, timer);
                });
            }
        }
    }

    TEST(TEST_CLASS, addPartialAddedNotifiers){
        //Timer
        boost::asio::io_context io;
        boost::asio::deadline_timer timer(io, boost::posix_time::seconds(1));

        // Create new account
        const std::string defaultAccountPrivateKey = "3E86205091D90B661F95E986D7CC63D68FB11227E8AAF469A5612DB62F89606A";
        std::shared_ptr<Account> defaultAccount = getTestAccount(defaultAccountPrivateKey);

        //Create transaction
        Mosaic mosaic(-4613020131619586570, 100);
        MosaicContainer mosaics{ mosaic };
        RawBuffer message("test message");
        EmbeddedTransactions embeddedTransactions;
        std::unordered_map<Key, Signature> coSignatures;

        //Create first embedded transaction
        std::unique_ptr<EmbeddedTransferTransaction> firstEmbeddedTransferTransaction =
                CreateEmbeddedTransferTransaction(account->address(), mosaics, message, defaultAccount->publicKey());

        //Create second embedded transaction
        std::unique_ptr<EmbeddedTransferTransaction> secondEmbeddedTransferTransaction =
                CreateEmbeddedTransferTransaction(defaultAccount->address(), mosaics, message, account->publicKey());
        embeddedTransactions.push_back(std::move(firstEmbeddedTransferTransaction));
        embeddedTransactions.push_back(std::move(secondEmbeddedTransferTransaction));

        //Sign transaction
        std::unique_ptr<AggregateTransaction> aggregateTransaction = CreateBondedAggregateTransaction(embeddedTransactions, coSignatures);
        account->signTransaction(aggregateTransaction.get());
        coSignatures[account->publicKey()] = account->cosignAggregateTransaction(aggregateTransaction.get());
        coSignatures[defaultAccount->publicKey()] = defaultAccount->cosignAggregateTransaction(aggregateTransaction.get());

        //Variables
        const int maxIterations = 15;
        int currentIteration = 0;
        bool isReceived = false;
        std::cout << ToHex(aggregateTransaction->hash()) << std::endl;
        
        //Handler
        PartialAddedNotifier notifier = [&aggregateTransaction, &isReceived](std::shared_ptr<transactions_info::BasicTransaction> transaction){
//            if (transaction->type == ToHex(aggregateTransaction->hash())){
//                EXPECT_EQ(ToHex(aggregateTransaction->hash()), transaction.meta.hash);
//
//                auto transactionInfo = client->transactions()->getAnyTransactionInfo(transaction.meta.hash);
//                EXPECT_EQ(TransactionType::Transfer,transactionInfo.get()->type);
//
//                isReceived = true;
//            }
            // std::cout << GetTransactionName(transaction->type);
            isReceived = true;
        };
        client->notifications()->addPartialAddedNotifiers(account->address(), {notifier});
        EXPECT_TRUE(client->transactions()->announceAggregateBoundedTransaction(aggregateTransaction->binary()));

        timer.async_wait([currentIteration, &isReceived, &timer](const boost::system::error_code& errorCode) {
            EXPECT_FALSE(errorCode.value());
            waitPartialAdded(currentIteration, maxIterations, isReceived, account->address(), timer);
        }); 
            
        io.run();
        EXPECT_TRUE(isReceived);
    }
}