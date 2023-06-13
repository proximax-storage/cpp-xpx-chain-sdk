/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/config.h>
#include <xpxchaincpp/model/notification/notifier.h>
#include <deque>
#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <thread>
#include <boost/asio/io_context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core/error.hpp>

namespace xpx_chain_sdk::internal::network {
    class Context;
    class WsClient;
}

namespace xpx_chain_sdk::internal {
    class SubscriptionManager;
}

namespace xpx_chain_sdk {

    using internal::network::Context;
    using notifierId = std::string;

class NotificationService : public std::enable_shared_from_this<NotificationService> {
    public:
        NotificationService(
                const Config& config,
                std::shared_ptr<internal::network::Context> context);
        ~NotificationService();

    void addBlockNotifiers(const std::vector<Notifier<Block>>& notifiers,
                           std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError);

    void removeBlockNotifiers(std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError,
                              const std::vector<notifierId>& notifierIds = {});

    void addConfirmedAddedNotifiers(const Address& address, const std::vector<Notifier<TransactionNotification>>& notifiers,
                                    std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError);

    void removeConfirmedAddedNotifiers(const Address& address, std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError,
                                       const std::vector<notifierId>& notifierIds = {});

    void addUnconfirmedAddedNotifiers(const Address& address, const std::vector<Notifier<TransactionNotification>>& notifiers,
                                      std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError);

    void removeUnconfirmedAddedNotifiers(const Address& address, std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError,
                                         const std::vector<notifierId>& notifierIds = {});

    void addUnconfirmedRemovedNotifiers(const Address& address, const std::vector<Notifier<transactions_info::TransactionInfo>>& notifiers,
                                        std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError);

    void removeUnconfirmedRemovedNotifiers(const Address& address, std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError,
                                           const std::vector<notifierId>& notifierIds = {});

    void addPartialAddedNotifiers(const Address& address, const std::vector<Notifier<std::shared_ptr<transactions_info::BasicTransaction>>>& notifiers,
                                  std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError);

    void removePartialAddedNotifiers(const Address& address, std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError,
                                     const std::vector<notifierId>& notifierIds = {});

    void addPartialRemovedNotifiers(const Address& address, const std::vector<Notifier<transactions_info::TransactionInfo>>& notifiers,
                                    std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError);

    void removePartialRemovedNotifiers(const Address& address, std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError,
                                       const std::vector<notifierId>& notifierIds = {});

    void addStatusNotifiers(const Address& address, const std::vector<Notifier<TransactionStatusNotification>>& notifiers,
                            std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError);

    void removeStatusNotifiers(const Address& address, std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError,
                               const std::vector<notifierId>& notifierIds = {});

    void addCosignatureNotifiers(const Address& address, const std::vector<Notifier<SignerInfoNotification>>& notifiers,
                                 std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError);

    void removeCosignatureNotifiers(const Address& address, std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError,
                                    const std::vector<notifierId>& notifierIds = {});

    void addDriveStateNotifiers(const Address& address, const std::vector<Notifier<DriveStateNotification>>& notifiers,
                                std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError);

    void removeDriveStateNotifiers(const Address& address, std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError,
                                   const std::vector<notifierId>& notifierIds = {});

    void connect(std::function<void(const std::string& message, int code)> callback);

    private:
        void run();
        void runTask(std::function<void()>);
        void notificationsHandler(const std::string& json);
        void stop();

        template<typename InternalContainer, typename ExternalContainer>
        void addNotifiers(const Address&, const std::string&, InternalContainer&, const ExternalContainer&,
                          std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError);

        template<typename InternalContainer, typename ExternalContainer>
        void removeNotifiers(const Address&, const std::string&, InternalContainer&, const ExternalContainer&,
                             std::function<void()> onSuccess, std::function<void(boost::beast::error_code errorCode)> onError);

    private:
        const Config& _config;
        std::shared_ptr<internal::network::Context> _context;
        std::shared_ptr<internal::network::WsClient> _wsClient;
        std::shared_ptr<internal::SubscriptionManager> _subscriptionManager;
        std::shared_ptr<boost::asio::io_context> _io_context;
        std::shared_ptr<boost::asio::strand<boost::asio::io_context::executor_type>> _strand;
        std::deque<std::function<void()>> _tasks;
        std::thread _mainWorker;
        std::map<notifierId, Notifier<Block>> _blockNotifiers;
        std::map<std::string, std::map<notifierId, Notifier<TransactionNotification>>> _confirmedAddedNotifiers;
        std::map<std::string, std::map<notifierId, Notifier<TransactionNotification>>> _unconfirmedAddedNotifiers;
        std::map<std::string, std::map<notifierId, Notifier<transactions_info::TransactionInfo>>> _unconfirmedRemovedNotifiers;
        std::map<std::string, std::map<notifierId, Notifier<std::shared_ptr<transactions_info::BasicTransaction>>>> _partialAddedNotifiers;
        std::map<std::string, std::map<notifierId, Notifier<transactions_info::TransactionInfo>>> _partialRemovedNotifiers;
        std::map<std::string, std::map<notifierId, Notifier<TransactionStatusNotification>>> _statusNotifiers;
        std::map<std::string, std::map<notifierId, Notifier<SignerInfoNotification>>> _cosignatureNotifiers;
        std::map<std::string, std::map<notifierId, Notifier<DriveStateNotification>>> _driveStateNotifiers;
        std::function<void(const std::string& message, int code)> _errorsCallback;
        std::string _uid;
        bool _isConnectionInProgress;
    };
}