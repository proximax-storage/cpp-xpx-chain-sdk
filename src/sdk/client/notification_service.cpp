/*
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
*/

#include "xpxchaincpp/client/notification_service.h"
#include <infrastructure/utils/deserialization_json.h>
#include <infrastructure/json/parser.h>
#include <infrastructure/network/websocket.h>
#include <infrastructure/utils/subscription_manager.h>
#include <sdk/model/notification/uid.h>
#include <sdk/model/notification/meta.h>

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
            _statusNotifiers({}),
            _isInitialized(false) {}

    NotificationService::~NotificationService() {
        // TODO unsubscribe all
    }

    void NotificationService::addMutexIfNotExist(const std::string& mutexKey) {
        std::lock_guard<std::mutex> lock(_commonMutex);
        if (_notifiersMutexes.find(mutexKey) == _notifiersMutexes.end()) {
            _notifiersMutexes.insert(std::pair<std::string, std::unique_ptr<std::mutex>>(mutexKey, new std::mutex));
        }
    }

    void NotificationService::initialize(std::function<void()> initializeCallback) {
        auto connectionCallback = [pThis = shared_from_this(), initializeCallback](const auto& json, const auto& errorCode) {
            if (errorCode) {
                pThis->_isInitialized = false;
                // call initializeCallback
                // throw exception
            }

            Uid uid = from_json<Uid, UidDto>(json);
            pThis->_uid = uid.value;

            std::cout << "uid str: " << uid.value << std::endl;

            pThis->_subscriptionManager = std::make_shared<internal::SubscriptionManager>(pThis->_wsClient);
            initializeCallback();
        };

        auto receiverCallback = [pThis = shared_from_this()](const auto& json, const auto& errorCode) {
            if (errorCode) {
                pThis->_isInitialized = false;
                // throw exception
            }

            pThis->notificationsHandler(json);
        };

        _wsClient = std::make_shared<internal::network::WsClient>(_config, _context, connectionCallback, receiverCallback);
        _wsClient->connect();

        _isInitialized = true;
    }

    void NotificationService::notificationsHandler(const std::string &json) {
        std::cout << json << std::endl;

        const auto websocketInfo = from_json<WebsocketInfo, WebsocketInfoDto>(json);
        if (internal::pathBlock == websocketInfo.meta.channelName) {
            std::lock_guard<std::mutex> lock(*_notifiersMutexes[internal::pathBlock]);
            auto block = from_json<Block, BlockDto>(json);
            for (const auto& notifier : _blockNotifiers) {
                notifier(block);
            }
        } else if (internal::pathConfirmedAdded == websocketInfo.meta.channelName) {

        } else if (internal::pathUnconfirmedAdded == websocketInfo.meta.channelName) {

        } else if (internal::pathUnconfirmedRemoved == websocketInfo.meta.channelName) {

        } else if (internal::pathStatus == websocketInfo.meta.channelName) {
            std::lock_guard<std::mutex> lock(*_notifiersMutexes[internal::pathStatus]);

            auto transactionStatus = from_json<TransactionStatusNotification, TransactionStatusNotificationDto>(json);
            for (auto const& notifiers : _statusNotifiers) {
                for (const auto& notifier : notifiers.second) {
                    notifier(transactionStatus);
                }
            }
        } else if (internal::pathPartialAdded == websocketInfo.meta.channelName) {

        } else if (internal::pathPartialRemoved == websocketInfo.meta.channelName) {

        } else if (internal::pathCosignature == websocketInfo.meta.channelName) {

        } else if (internal::driveState == websocketInfo.meta.channelName) {

        } else {
            // unknown channel, throw exception
        }
    }

    void NotificationService::addBlockNotifiers(const std::initializer_list<BlockNotifier> &notifiers) {
        if (empty(notifiers)) {
            // throw exception
            return;
        }

        addMutexIfNotExist(internal::pathBlock);

        auto callback = [pThis = shared_from_this(), notifiers]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathBlock]);

            if (pThis->_blockNotifiers.empty()) {
                pThis->_blockNotifiers.insert(pThis->_blockNotifiers.end(), notifiers.begin(), notifiers.end());
                pThis->_subscriptionManager->subscribe(pThis->_uid, internal::pathBlock);
            } else {
                pThis->_blockNotifiers.insert(pThis->_blockNotifiers.end(), notifiers.begin(), notifiers.end());
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::removeBlockNotifiers() {
        addMutexIfNotExist(internal::pathBlock);
        auto callback = [pThis = shared_from_this()]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathBlock]);

            if (!pThis->_blockNotifiers.empty()) {
                pThis->_subscriptionManager->unsubscribe(pThis->_uid, internal::pathBlock);
                pThis->_blockNotifiers.clear();
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::addConfirmedAddedNotifiers(const std::string &address, const std::initializer_list<ConfirmedAddedNotifier> &notifiers) {
        if (empty(notifiers)) {
            // throw exception
            return;
        }

        addMutexIfNotExist(internal::pathConfirmedAdded);

        auto callback = [pThis = shared_from_this(), address, notifiers]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathConfirmedAdded]);

            if ( pThis->_confirmedAddedNotifiers.find(address) != pThis->_confirmedAddedNotifiers.end() ) {
                pThis->_confirmedAddedNotifiers[address].insert(pThis->_confirmedAddedNotifiers[address].end(), notifiers.begin(), notifiers.end());
            } else {
                pThis->_confirmedAddedNotifiers.insert(std::pair<std::string, std::vector<ConfirmedAddedNotifier>>(address, notifiers));
                pThis->_subscriptionManager->subscribe(pThis->_uid, internal::pathConfirmedAdded + "/" + address);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::removeConfirmedAddedNotifiers(const std::string &address) {
        addMutexIfNotExist(internal::pathConfirmedAdded);
        auto callback = [pThis = shared_from_this(), address]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathConfirmedAdded]);

            if (pThis->_confirmedAddedNotifiers.find(address) != pThis->_confirmedAddedNotifiers.end()) {
                pThis->_subscriptionManager->unsubscribe(pThis->_uid, internal::pathConfirmedAdded + "/" + address);
                pThis->_confirmedAddedNotifiers.erase(address);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::addUnconfirmedAddedNotifiers(const std::string &address, const std::initializer_list<UnconfirmedAddedNotifier> &notifiers) {
        if (empty(notifiers)) {
            // throw exception
            return;
        }

        addMutexIfNotExist(internal::pathUnconfirmedAdded);

        auto callback = [pThis = shared_from_this(), address, notifiers]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathUnconfirmedAdded]);

            if ( pThis->_unconfirmedAddedNotifiers.find(address) != pThis->_unconfirmedAddedNotifiers.end() ) {
                pThis->_unconfirmedAddedNotifiers[address].insert(pThis->_unconfirmedAddedNotifiers[address].end(), notifiers.begin(), notifiers.end());
            } else {
                pThis->_unconfirmedAddedNotifiers.insert(std::pair<std::string, std::vector<ConfirmedAddedNotifier>>(address, notifiers));
                pThis->_subscriptionManager->subscribe(pThis->_uid, internal::pathUnconfirmedAdded + "/" + address);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::removeUnconfirmedAddedNotifiers(const std::string &address) {
        addMutexIfNotExist(internal::pathUnconfirmedAdded);
        auto callback = [pThis = shared_from_this(), address]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathUnconfirmedAdded]);

            if (pThis->_unconfirmedAddedNotifiers.find(address) != pThis->_unconfirmedAddedNotifiers.end()) {
                pThis->_subscriptionManager->unsubscribe(pThis->_uid, internal::pathUnconfirmedAdded + "/" + address);
                pThis->_unconfirmedAddedNotifiers.erase(address);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::addUnconfirmedRemovedNotifiers(const std::string &address, const std::initializer_list<UnconfirmedRemovedNotifier> &notifiers) {
        if (empty(notifiers)) {
            // throw exception
            return;
        }

        addMutexIfNotExist(internal::pathUnconfirmedRemoved);

        auto callback = [pThis = shared_from_this(), address, notifiers]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathUnconfirmedRemoved]);

            if ( pThis->_unconfirmedRemovedNotifiers.find(address) != pThis->_unconfirmedRemovedNotifiers.end() ) {
                pThis->_unconfirmedRemovedNotifiers[address].insert(pThis->_unconfirmedRemovedNotifiers[address].end(), notifiers.begin(), notifiers.end());
            } else {
                pThis->_unconfirmedRemovedNotifiers.insert(std::pair<std::string, std::vector<UnconfirmedRemovedNotifier>>(address, notifiers));
                pThis->_subscriptionManager->subscribe(pThis->_uid, internal::pathUnconfirmedRemoved + "/" + address);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::removeUnconfirmedRemovedNotifiers(const std::string &address) {
        addMutexIfNotExist(internal::pathUnconfirmedRemoved);
        auto callback = [pThis = shared_from_this(), address]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathUnconfirmedRemoved]);

            if (pThis->_unconfirmedRemovedNotifiers.find(address) != pThis->_unconfirmedRemovedNotifiers.end()) {
                pThis->_subscriptionManager->unsubscribe(pThis->_uid, internal::pathUnconfirmedRemoved + "/" + address);
                pThis->_unconfirmedRemovedNotifiers.erase(address);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::addPartialAddedNotifiers(const std::string &address, const std::initializer_list<StatusNotifier> &notifiers) {

    }

    void NotificationService::removePartialAddedNotifiers(const std::string &address) {

    }

    void NotificationService::addPartialRemovedNotifiers(const std::string &address, const std::initializer_list<StatusNotifier> &notifiers) {

    }

    void NotificationService::removePartialRemovedNotifiers(const std::string &address) {

    }

    void NotificationService::addStatusNotifiers(const std::string& address, const std::initializer_list<StatusNotifier>& notifiers) {
        if (empty(notifiers)) {
            // throw exception
            return;
        }

        addMutexIfNotExist(internal::pathStatus);

        auto callback = [pThis = shared_from_this(), address, notifiers]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathStatus]);

            if ( pThis->_statusNotifiers.find(address) != pThis->_statusNotifiers.end() ) {
                pThis->_statusNotifiers[address].insert(pThis->_statusNotifiers[address].end(), notifiers.begin(), notifiers.end());
            } else {
                pThis->_statusNotifiers.insert(std::pair<std::string, std::vector<StatusNotifier>>(address, notifiers));
                pThis->_subscriptionManager->subscribe(pThis->_uid, internal::pathStatus + "/" + address);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::removeStatusNotifiers(const std::string &address) {
        addMutexIfNotExist(internal::pathStatus);
        auto callback = [pThis = shared_from_this(), address]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathStatus]);

            if (pThis->_statusNotifiers.find(address) != pThis->_statusNotifiers.end()) {
                pThis->_subscriptionManager->unsubscribe(pThis->_uid, internal::pathStatus + "/" + address);
                pThis->_statusNotifiers.erase(address);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::addCosignatureNotifiers(const std::string &address, const std::initializer_list<StatusNotifier> &notifiers) {

    }

    void NotificationService::removeCosignatureNotifiers(const std::string &address) {

    }

    void NotificationService::addDriveStateNotifiers(const std::string &address, const std::initializer_list<StatusNotifier> &notifiers) {

    }

    void NotificationService::removeDriveStateNotifiers(const std::string &address) {

    }
}