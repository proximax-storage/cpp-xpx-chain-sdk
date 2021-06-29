/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/config.h>
#include <xpxchaincpp/model/blockchain/block.h>
#include <xpxchaincpp/model/transaction_simple/transaction.h>
#include <xpxchaincpp/model/transaction_simple/transaction_info.h>
#include <xpxchaincpp/model/notification/drive_state_notification.h>
#include <xpxchaincpp/model/notification/signer_info_notification.h>
#include <xpxchaincpp/model/notification/transaction_notification.h>
#include <xpxchaincpp/model/notification/transaction_status_notification.h>
#include <condition_variable>
#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <mutex>
#include <initializer_list>

namespace xpx_chain_sdk::internal::network {
    class Context;
    class WsClient;
}

namespace xpx_chain_sdk::internal {
    class SubscriptionManager;
}

namespace xpx_chain_sdk {

    using internal::network::Context;
    using BlockNotifier = std::function<void(const Block& block)>;
    using ConfirmedAddedNotifier = std::function<void(const TransactionNotification& transactionNotification)>;
    using UnconfirmedAddedNotifier = std::function<void(const TransactionNotification& transactionNotification)>;
    using UnconfirmedRemovedNotifier = std::function<void(const transactions_info::TransactionInfo& transactionInfo)>;
    using PartialAddedNotifier = std::function<void(std::shared_ptr<transactions_info::BasicTransaction> transaction)>;
    using PartialRemovedNotifier = std::function<void(const transactions_info::TransactionInfo& transactionInfo)>;
    using StatusNotifier = std::function<void(const TransactionStatusNotification& status)>;
    using CosignatureNotifier = std::function<void(const SignerInfoNotification& signerInfo)>;
    using DriveStateNotifier = std::function<void(const DriveStateNotification& driveState)>;

class NotificationService : public std::enable_shared_from_this<NotificationService> {
    public:
        NotificationService(
                std::shared_ptr<Config> config,
                std::shared_ptr<internal::network::Context> context);
        ~NotificationService();

    void addBlockNotifiers(const std::initializer_list<BlockNotifier>& notifiers);
    void removeBlockNotifiers();

    void addConfirmedAddedNotifiers(const Address& address, const std::initializer_list<ConfirmedAddedNotifier>& notifiers);
    void removeConfirmedAddedNotifiers(const Address& address);

    void addUnconfirmedAddedNotifiers(const Address& address, const std::initializer_list<UnconfirmedAddedNotifier>& notifiers);
    void removeUnconfirmedAddedNotifiers(const Address& address);

    void addUnconfirmedRemovedNotifiers(const Address& address, const std::initializer_list<UnconfirmedRemovedNotifier>& notifiers);
    void removeUnconfirmedRemovedNotifiers(const Address& address);

    void addPartialAddedNotifiers(const Address& address, const std::initializer_list<PartialAddedNotifier>& notifiers);
    void removePartialAddedNotifiers(const Address& address);

    void addPartialRemovedNotifiers(const Address& address, const std::initializer_list<PartialRemovedNotifier>& notifiers);
    void removePartialRemovedNotifiers(const Address& address);

    void addStatusNotifiers(const Address& address, const std::initializer_list<StatusNotifier>& notifiers);
    void removeStatusNotifiers(const Address& address);

    void addCosignatureNotifiers(const Address& address, const std::initializer_list<CosignatureNotifier>& notifiers);
    void removeCosignatureNotifiers(const Address& address);

    void addDriveStateNotifiers(const Address& address, const std::initializer_list<DriveStateNotifier>& notifiers);
    void removeDriveStateNotifiers(const Address& address);

    private:
        void addMutexIfNotExist(const std::string& mutexKey);
        void initialize(std::function<void()> initializeCallback);
        void notificationsHandler(const std::string& json);
        void finalize();

    private:
        std::shared_ptr<Config> _config;
        std::shared_ptr<internal::network::Context> _context;
        std::shared_ptr<internal::network::WsClient> _wsClient;
        std::shared_ptr<internal::SubscriptionManager> _subscriptionManager;
        std::map<std::string, std::unique_ptr<std::mutex>> _notifiersMutexes;
        std::vector<BlockNotifier> _blockNotifiers;
        std::map<std::string, std::vector<ConfirmedAddedNotifier>> _confirmedAddedNotifiers;
        std::map<std::string, std::vector<UnconfirmedAddedNotifier>> _unconfirmedAddedNotifiers;
        std::map<std::string, std::vector<UnconfirmedRemovedNotifier>> _unconfirmedRemovedNotifiers;
        std::map<std::string, std::vector<PartialAddedNotifier>> _partialAddedNotifiers;
        std::map<std::string, std::vector<PartialRemovedNotifier>> _partialRemovedNotifiers;
        std::map<std::string, std::vector<StatusNotifier>> _statusNotifiers;
        std::map<std::string, std::vector<CosignatureNotifier>> _cosignatureNotifiers;
        std::map<std::string, std::vector<DriveStateNotifier>> _driveStateNotifiers;
        std::mutex _commonMutex;
        std::mutex _initializeMutex;
        std::condition_variable _initializeCheck;
        std::string _uid;
        bool _isInitialized;
    };
}