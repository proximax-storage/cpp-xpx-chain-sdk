/*
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include <xpxchaincpp/client/notification_service.h>
#include <xpxchaincpp/exceptions.h>
#include <infrastructure/json/parser.h>
#include <infrastructure/utils/deserialization_json.h>
#include <infrastructure/utils/subscription_manager.h>
#include <infrastructure/network/websocket.h>
#include <sdk/model/notification/websocket_uid.h>
#include <sdk/model/notification/websocket_meta.h>

using namespace xpx_chain_sdk;

using namespace xpx_chain_sdk::internal::json::dto;
using internal::json::dto::BlockDto;
using xpx_chain_sdk::internal::json::Parser;
using xpx_chain_sdk::internal::json::dto::from_json;

namespace xpx_chain_sdk {

    NotificationService::NotificationService(
            std::shared_ptr<Config> config,
            std::shared_ptr<internal::network::Context> context):
            _config(config),
            _context(context),
            _wsClient(nullptr),
            _subscriptionManager(nullptr),
            _io_context(std::make_shared<boost::asio::io_context>()),
            _strand(std::make_shared<boost::asio::strand<boost::asio::io_context::executor_type>>(boost::asio::make_strand(_io_context->get_executor()))),
            _tasks(),
            _mainWorker(),
            _isConnectionInProgress(false) {
        run();
    }

    NotificationService::~NotificationService() {
        stop();
    }

    void NotificationService::run() {
        boost::asio::post(*_strand, [this] {
            auto connectionCallback = [this](const auto& json) {
                Uid uid = from_json<Uid, UidDto>(json);
                _uid = uid.value;

                _isConnectionInProgress = false;

                for (const auto& task : _tasks) {
                    task();
                }

                _tasks.clear();
            };

            auto receiverCallback = [this](const auto& json) {
                notificationsHandler(json);
            };

            auto errorCallback = [this](const boost::beast::error_code& errorCode) {
                if (errorCode) {
                    std::string errorMessage = errorCode.message();
                    if (boost::asio::error::eof == errorCode) {
                        errorMessage = "connection closed by server side";
                    }

                    _isConnectionInProgress = false;
                    _tasks.clear();
                    XPX_CHAIN_SDK_THROW_1(notification_error, errorMessage, errorCode.value())
                }
            };

            _isConnectionInProgress = true;
            _wsClient = std::make_shared<internal::network::WsClient>(_config, _context, _strand, connectionCallback, receiverCallback, errorCallback);
            _subscriptionManager = std::make_shared<internal::SubscriptionManager>(_wsClient);
            _wsClient->connect();
        });

        _mainWorker = std::thread([this]() { _io_context->run(); });
    }

    void NotificationService::runTask(std::function<void()> task) {
        boost::asio::post(*_strand, [pThis = shared_from_this(), task] {
            if (pThis->_isConnectionInProgress) {
                pThis->_tasks.emplace_back(task);
            } else {
                task();
            }
        });
    }

    void NotificationService::stop() {
        boost::asio::post(*_strand, [this] {
            if (_wsClient->isConnected()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathBlock);

                for (auto const &item: _confirmedAddedNotifiers) {
                    _subscriptionManager->unsubscribe(_uid, internal::pathConfirmedAdded, Address::FromHex(item.first));
                }

                for (auto const &item: _unconfirmedAddedNotifiers) {
                    _subscriptionManager->unsubscribe(_uid, internal::pathUnconfirmedAdded, Address::FromHex(item.first));
                }

                for (auto const &item: _unconfirmedRemovedNotifiers) {
                    _subscriptionManager->unsubscribe(_uid, internal::pathUnconfirmedRemoved, Address::FromHex(item.first));
                }

                for (auto const &item: _partialAddedNotifiers) {
                    _subscriptionManager->unsubscribe(_uid, internal::pathPartialAdded, Address::FromHex(item.first));
                }

                for (auto const &item: _partialRemovedNotifiers) {
                    _subscriptionManager->unsubscribe(_uid, internal::pathPartialRemoved, Address::FromHex(item.first));
                }

                for (auto const &item: _statusNotifiers) {
                    _subscriptionManager->unsubscribe(_uid, internal::pathStatus, Address::FromHex(item.first));
                }

                for (auto const &item: _cosignatureNotifiers) {
                    _subscriptionManager->unsubscribe(_uid, internal::pathCosignature, Address::FromHex(item.first));
                }

                for (auto const &item: _driveStateNotifiers) {
                    _subscriptionManager->unsubscribe(_uid, internal::pathDriveState, Address::FromHex(item.first));
                }

                _wsClient->disconnect();
            }
        });

        _mainWorker.join();
    }

    template<typename InternalContainer, typename ExternalContainer>
    void NotificationService::addNotifiers(
            const Address& address,
            const std::string& path,
            InternalContainer& linkedNotifiers,
            const ExternalContainer& notifiers) {
        if (empty(notifiers)) {
            XPX_CHAIN_SDK_THROW_1(notification_error, "list of notifiers is empty", __FUNCTION__)
        }

        auto task = [pThis = shared_from_this(), address, path, &linkedNotifiers, notifiers]() {
            const std::string hexAddress = ToHex(address.binary());
            if ( linkedNotifiers.find(hexAddress) != linkedNotifiers.end() ) {
                for (const auto& notifier : notifiers) {
                    linkedNotifiers[hexAddress].insert({notifier.getId(), notifier});
                }
            } else {
                linkedNotifiers.insert({hexAddress, {}});

                for (const auto& notifier : notifiers) {
                    linkedNotifiers[hexAddress].insert({notifier.getId(), notifier});
                }

                pThis->_subscriptionManager->subscribe(pThis->_uid, path, address);
            }
        };

        runTask(task);
    }

    template<typename InternalContainer, typename ExternalContainer>
    void NotificationService::removeNotifiers(
            const Address& address,
            const std::string& path,
            InternalContainer& linkedNotifiers,
            const ExternalContainer& notifierIds) {
        auto task = [pThis = shared_from_this(), address, path, &linkedNotifiers, notifierIds]() {
            const std::string hexAddress = ToHex(address.binary());
            if (linkedNotifiers.find(hexAddress) != linkedNotifiers.end()) {
                for (const auto& id : notifierIds) {
                    linkedNotifiers[hexAddress].erase(id);
                }

                if (notifierIds.empty()) {
                    linkedNotifiers[hexAddress].clear();
                }

                if (linkedNotifiers[hexAddress].empty()) {
                    linkedNotifiers.erase(hexAddress);
                    pThis->_subscriptionManager->unsubscribe(pThis->_uid, path, address);
                }
            }
        };

        runTask(task);
    }

    void NotificationService::notificationsHandler(const std::string &json) {
        auto websocketInfo = from_json<WebsocketInfo, WebsocketInfoDto>(json);
        
        if (internal::pathBlock == websocketInfo.meta.channelName) {
            if (_blockNotifiers.empty()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathBlock);
            } else {
                auto block = from_json<Block, BlockDto>(json);
                for (const auto& notifier : _blockNotifiers) {
                    notifier.second.run(block);
                }
            }
        } else if (internal::pathConfirmedAdded == websocketInfo.meta.channelName) {
            if (_confirmedAddedNotifiers.find(websocketInfo.meta.address) == _confirmedAddedNotifiers.end()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathConfirmedAdded, Address::FromHex(websocketInfo.meta.address));
            } else {
                auto transaction = from_json<TransactionNotification, TransactionNotificationDto>(json);
                for (const auto& notifier : _confirmedAddedNotifiers[websocketInfo.meta.address]) {
                    notifier.second.run(transaction);
                }
            }
        } else if (internal::pathUnconfirmedAdded == websocketInfo.meta.channelName) {
            if (_unconfirmedAddedNotifiers.find(websocketInfo.meta.address) == _unconfirmedAddedNotifiers.end()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathUnconfirmedAdded, Address::FromHex(websocketInfo.meta.address));
            } else {
                auto transaction = from_json<TransactionNotification, TransactionNotificationDto>(json);
                for (const auto& notifier : _unconfirmedAddedNotifiers[websocketInfo.meta.address]) {
                    notifier.second.run(transaction);
                }
            }
        } else if (internal::pathUnconfirmedRemoved == websocketInfo.meta.channelName) {
            if (_unconfirmedRemovedNotifiers.find(websocketInfo.meta.address) == _unconfirmedRemovedNotifiers.end()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathUnconfirmedRemoved, Address::FromHex(websocketInfo.meta.address));
            } else {
                auto transactionInfo = from_json<transactions_info::TransactionInfo, TransactionInfoDto>(json);
                for (const auto& notifier : _unconfirmedRemovedNotifiers[websocketInfo.meta.address]) {
                    notifier.second.run(transactionInfo);
                }
            }
        } else if (internal::pathStatus == websocketInfo.meta.channelName) {
            if (_statusNotifiers.find(websocketInfo.meta.address) == _statusNotifiers.end()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathStatus, Address::FromHex(websocketInfo.meta.address));
            } else {
                auto transactionStatus = from_json<TransactionStatusNotification, TransactionStatusNotificationDto>(json);
                for (const auto& notifier : _statusNotifiers[websocketInfo.meta.address]) {
                    notifier.second.run(transactionStatus);
                }
            }
        } else if (internal::pathPartialAdded == websocketInfo.meta.channelName) {
            if (_partialAddedNotifiers.find(websocketInfo.meta.address) == _partialAddedNotifiers.end()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathPartialAdded, Address::FromHex(websocketInfo.meta.address));
            } else {
                auto transaction = transaction_from_json(json);
                for (const auto& notifier : _partialAddedNotifiers[websocketInfo.meta.address]) {
                    notifier.second.run(transaction);
                }
            }
        } else if (internal::pathPartialRemoved == websocketInfo.meta.channelName) {
            if (_partialRemovedNotifiers.find(websocketInfo.meta.address) == _partialRemovedNotifiers.end()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathPartialRemoved, Address::FromHex(websocketInfo.meta.address));
            } else {
                auto transactionInfo = from_json<transactions_info::TransactionInfo, TransactionInfoDto>(json);
                for (const auto& notifier : _partialRemovedNotifiers[websocketInfo.meta.address]) {
                    notifier.second.run(transactionInfo);
                }
            }
        } else if (internal::pathCosignature == websocketInfo.meta.channelName) {
            if (_cosignatureNotifiers.find(websocketInfo.meta.address) == _cosignatureNotifiers.end()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathCosignature, Address::FromHex(websocketInfo.meta.address));
            } else {
                auto signerInfo = from_json<SignerInfoNotification, SignerInfoNotificationDto>(json);
                for (const auto& notifier : _cosignatureNotifiers[websocketInfo.meta.address]) {
                    notifier.second.run(signerInfo);
                }
            }
        } else if (internal::pathDriveState == websocketInfo.meta.channelName) {
            if (_driveStateNotifiers.find(websocketInfo.meta.address) == _driveStateNotifiers.end()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathDriveState, Address::FromHex(websocketInfo.meta.address));
            } else {
                auto driveState = from_json<DriveStateNotification, DriveStateNotificationDto>(json);
                for (const auto& notifier : _driveStateNotifiers[websocketInfo.meta.address]) {
                    notifier.second.run(driveState);
                }
            }
        } else {
            std::cout << "notifications: received notification from unsupported channel: " << json << " (Ignored)" << std::endl;
        }
    }

    void NotificationService::addBlockNotifiers(const std::vector<Notifier<Block>>& notifiers) {
        if (empty(notifiers)) {
            XPX_CHAIN_SDK_THROW_1(notification_error, "list of notifiers is empty", __FUNCTION__)
        }

        auto task = [pThis = shared_from_this(), notifiers]() {
            const bool isEmpty = pThis->_blockNotifiers.empty();

            for (const auto& notifier : notifiers) {
                pThis->_blockNotifiers.insert({notifier.getId(), notifier});
            }

            if (isEmpty) {
                pThis->_subscriptionManager->subscribe(pThis->_uid, internal::pathBlock);
            }
        };

        runTask(task);
    }

    void NotificationService::removeBlockNotifiers(const std::vector<notifierId>& notifierIds) {
        auto task = [pThis = shared_from_this(), notifierIds]() {
            for (const auto& id : notifierIds) {
                pThis->_blockNotifiers.erase(id);
            }

            if (notifierIds.empty()) {
                pThis->_blockNotifiers.clear();
            }

            if (pThis->_blockNotifiers.empty()) {
                pThis->_subscriptionManager->unsubscribe(pThis->_uid, internal::pathBlock);
            }
        };

        runTask(task);
    }

    void NotificationService::addConfirmedAddedNotifiers(const Address& address, const std::vector<Notifier<TransactionNotification>>& notifiers) {
        addNotifiers(address, internal::pathConfirmedAdded, _confirmedAddedNotifiers, notifiers);
    }

    void NotificationService::removeConfirmedAddedNotifiers(const Address& address, const std::vector<notifierId>& notifierIds) {
        removeNotifiers(address, internal::pathConfirmedAdded, _confirmedAddedNotifiers, notifierIds);
    }

    void NotificationService::addUnconfirmedAddedNotifiers(const Address& address, const std::vector<Notifier<TransactionNotification>>& notifiers) {
        addNotifiers(address, internal::pathUnconfirmedAdded, _unconfirmedAddedNotifiers, notifiers);
    }

    void NotificationService::removeUnconfirmedAddedNotifiers(const Address& address, const std::vector<notifierId>& notifierIds) {
        removeNotifiers(address, internal::pathUnconfirmedAdded, _unconfirmedAddedNotifiers, notifierIds);
    }

    void NotificationService::addUnconfirmedRemovedNotifiers(const Address& address, const std::vector<Notifier<transactions_info::TransactionInfo>>& notifiers) {
        addNotifiers(address, internal::pathUnconfirmedRemoved, _unconfirmedRemovedNotifiers, notifiers);
    }

    void NotificationService::removeUnconfirmedRemovedNotifiers(const Address& address, const std::vector<notifierId>& notifierIds) {
        removeNotifiers(address, internal::pathUnconfirmedRemoved, _unconfirmedRemovedNotifiers, notifierIds);
    }

    void NotificationService::addPartialAddedNotifiers(const Address& address, const std::vector<Notifier<std::shared_ptr<transactions_info::BasicTransaction>>>& notifiers) {
        addNotifiers(address, internal::pathPartialAdded, _partialAddedNotifiers, notifiers);
    }

    void NotificationService::removePartialAddedNotifiers(const Address& address, const std::vector<notifierId>& notifierIds) {
        removeNotifiers(address, internal::pathPartialAdded, _partialAddedNotifiers, notifierIds);
    }

    void NotificationService::addPartialRemovedNotifiers(const Address& address, const std::vector<Notifier<transactions_info::TransactionInfo>>& notifiers) {
        addNotifiers(address, internal::pathPartialRemoved, _partialRemovedNotifiers, notifiers);
    }

    void NotificationService::removePartialRemovedNotifiers(const Address& address, const std::vector<notifierId>& notifierIds) {
        removeNotifiers(address, internal::pathPartialRemoved, _partialRemovedNotifiers, notifierIds);
    }

    void NotificationService::addStatusNotifiers(const Address& address, const std::vector<Notifier<TransactionStatusNotification>>& notifiers) {
        addNotifiers(address, internal::pathStatus, _statusNotifiers, notifiers);
    }

    void NotificationService::removeStatusNotifiers(const Address& address, const std::vector<notifierId>& notifierIds) {
        removeNotifiers(address, internal::pathStatus, _statusNotifiers, notifierIds);
    }

    void NotificationService::addCosignatureNotifiers(const Address& address, const std::vector<Notifier<SignerInfoNotification>>& notifiers) {
        addNotifiers(address, internal::pathCosignature, _cosignatureNotifiers, notifiers);
    }

    void NotificationService::removeCosignatureNotifiers(const Address& address, const std::vector<notifierId>& notifierIds) {
        removeNotifiers(address, internal::pathCosignature, _cosignatureNotifiers, notifierIds);
    }

    void NotificationService::addDriveStateNotifiers(const Address& address, const std::vector<Notifier<DriveStateNotification>>& notifiers) {
        addNotifiers(address, internal::pathDriveState, _driveStateNotifiers, notifiers);
    }

    void NotificationService::removeDriveStateNotifiers(const Address& address, const std::vector<notifierId>& notifierIds) {
        removeNotifiers(address, internal::pathDriveState, _driveStateNotifiers, notifierIds);
    }
}