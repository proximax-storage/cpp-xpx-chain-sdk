/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/config.h>
#include <xpxchaincpp/model/blockchain/block.h>
#include <xpxchaincpp/model/transaction_simple/transaction.h>
#include <xpxchaincpp/model/notification/transaction_status_notification.h>
#include <memory>
#include <vector>
#include <functional>
#include <map>
#include <mutex>
#include <initializer_list>
#include <xpxchaincpp/model/transaction_simple/transaction_info.h>

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
    using ConfirmedAddedNotifier = std::function<void(const transactions_info::Transaction& transaction)>;
    using UnconfirmedAddedNotifier = ConfirmedAddedNotifier;
    using UnconfirmedRemovedNotifier = std::function<void(const transactions_info::TransactionInfo& transaction)>;
    using StatusNotifier = std::function<void(const TransactionStatusNotification& status)>;

class NotificationService : public std::enable_shared_from_this<NotificationService> {
    public:
        NotificationService(
                std::shared_ptr<Config> config,
                std::shared_ptr<internal::network::Context> context);
        ~NotificationService();

    void addBlockNotifiers(const std::initializer_list<BlockNotifier>& notifiers);
    void removeBlockNotifiers();

    void addConfirmedAddedNotifiers(const std::string& address, const std::initializer_list<ConfirmedAddedNotifier>& notifiers);
    void removeConfirmedAddedNotifiers(const std::string& address);

    void addUnconfirmedAddedNotifiers(const std::string& address, const std::initializer_list<UnconfirmedAddedNotifier>& notifiers);
    void removeUnconfirmedAddedNotifiers(const std::string& address);

    void addUnconfirmedRemovedNotifiers(const std::string& address, const std::initializer_list<UnconfirmedRemovedNotifier>& notifiers);
    void removeUnconfirmedRemovedNotifiers(const std::string& address);

    void addPartialAddedNotifiers(const std::string& address, const std::initializer_list<StatusNotifier>& notifiers);
    void removePartialAddedNotifiers(const std::string& address);

    void addPartialRemovedNotifiers(const std::string& address, const std::initializer_list<StatusNotifier>& notifiers);
    void removePartialRemovedNotifiers(const std::string& address);

    void addStatusNotifiers(const std::string& address, const std::initializer_list<StatusNotifier>& notifiers);
    void removeStatusNotifiers(const std::string& address);

    void addCosignatureNotifiers(const std::string& address, const std::initializer_list<StatusNotifier>& notifiers);
    void removeCosignatureNotifiers(const std::string& address);

    void addDriveStateNotifiers(const std::string& address, const std::initializer_list<StatusNotifier>& notifiers);
    void removeDriveStateNotifiers(const std::string& address);

    private:
        void addMutexIfNotExist(const std::string& mutexKey);
        void initialize(std::function<void()> initializeCallback);
        void notificationsHandler(const std::string& json);

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
        std::map<std::string, std::vector<StatusNotifier>> _statusNotifiers;
        std::mutex _commonMutex;
        std::string _uid;
        bool _isInitialized;
    };
}