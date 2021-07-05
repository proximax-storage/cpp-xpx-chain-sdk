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
#include <chrono>
#include <thread>

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
        finalize();
    }

    void NotificationService::addMutexIfNotExist(const std::string& mutexKey) {
        std::lock_guard<std::mutex> lock(_commonMutex);
        if (_notifiersMutexes.find(mutexKey) == _notifiersMutexes.end()) {
            _notifiersMutexes.insert(std::pair<std::string, std::unique_ptr<std::mutex>>(mutexKey, new std::mutex));
        }
    }

    void NotificationService::initialize(std::function<void()> initializeCallback) {
        std::unique_lock<std::mutex> lock(_initializeMutex);
        auto connectionCallback = [pThis = shared_from_this(), initializeCallback](const auto& json) {
            Uid uid = from_json<Uid, UidDto>(json);
            pThis->_uid = uid.value;

            pThis->_subscriptionManager = std::make_shared<internal::SubscriptionManager>(pThis->_wsClient);
            pThis->_isInitialized = true;
            initializeCallback();
            pThis->_initializeCheck.notify_all();
        };

        auto receiverCallback = [pThis = shared_from_this()](const auto& json) {
            pThis->notificationsHandler(json);
        };

        auto errorCallback = [pThis = shared_from_this()](const boost::beast::error_code& errorCode) {
            if (errorCode) {
                std::string errorMessage = errorCode.message();
                if (boost::asio::error::eof == errorCode) {
                    errorMessage = "connection closed by server side";
                }

                pThis->_isInitialized = false;
                pThis->_initializeCheck.notify_all();
                XPX_CHAIN_SDK_THROW_1(notification_error, errorMessage, errorCode.value())
            }
        };

        _wsClient = std::make_shared<internal::network::WsClient>(_config, _context, connectionCallback, receiverCallback, errorCallback);
        _wsClient->connect();
        _initializeCheck.wait_for(lock, std::chrono::seconds(60), [pThis = shared_from_this()]() {
            return pThis->_wsClient->isConnected();
        });
    }

    void NotificationService::finalize() {
        if (_isInitialized) {
            _subscriptionManager->unsubscribe(_uid, internal::pathBlock);

            for (auto const& item : _confirmedAddedNotifiers)
            {
                _subscriptionManager->unsubscribe(_uid, internal::pathConfirmedAdded, Address::FromHex(item.first));
            }

            for (auto const& item : _unconfirmedAddedNotifiers)
            {
                _subscriptionManager->unsubscribe(_uid, internal::pathUnconfirmedAdded, Address::FromHex(item.first));
            }

            for (auto const& item : _unconfirmedRemovedNotifiers)
            {
                _subscriptionManager->unsubscribe(_uid, internal::pathUnconfirmedRemoved, Address::FromHex(item.first));
            }

            for (auto const& item : _partialAddedNotifiers)
            {
                _subscriptionManager->unsubscribe(_uid, internal::pathPartialAdded, Address::FromHex(item.first));
            }

            for (auto const& item : _partialRemovedNotifiers)
            {
                _subscriptionManager->unsubscribe(_uid, internal::pathPartialRemoved, Address::FromHex(item.first));
            }

            for (auto const& item : _statusNotifiers)
            {
                _subscriptionManager->unsubscribe(_uid, internal::pathStatus, Address::FromHex(item.first));
            }

            for (auto const& item : _cosignatureNotifiers)
            {
                _subscriptionManager->unsubscribe(_uid, internal::pathCosignature, Address::FromHex(item.first));
            }

            for (auto const& item : _driveStateNotifiers)
            {
                _subscriptionManager->unsubscribe(_uid, internal::pathDriveState, Address::FromHex(item.first));
            }

            _wsClient->disconnect();
        }
    }

    void NotificationService::notificationsHandler(const std::string &json) {
        auto websocketInfo = from_json<WebsocketInfo, WebsocketInfoDto>(json);
        
        if (internal::pathBlock == websocketInfo.meta.channelName) {
            if (_blockNotifiers.empty()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathBlock);
            } else {
                auto block = from_json<Block, BlockDto>(json);
                for (const auto& notifier : _blockNotifiers) {
                    notifier(block);
                }
            }
        } else if (internal::pathConfirmedAdded == websocketInfo.meta.channelName) {
            if (_confirmedAddedNotifiers.find(websocketInfo.meta.address) == _confirmedAddedNotifiers.end()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathConfirmedAdded, Address::FromHex(websocketInfo.meta.address));
            } else {
                auto transaction = from_json<TransactionNotification, TransactionNotificationDto>(json);
                for (const auto& notifier : _confirmedAddedNotifiers[websocketInfo.meta.address]) {
                    notifier(transaction);
                }
            }
        } else if (internal::pathUnconfirmedAdded == websocketInfo.meta.channelName) {
            if (_unconfirmedAddedNotifiers.find(websocketInfo.meta.address) == _unconfirmedAddedNotifiers.end()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathUnconfirmedAdded, Address::FromHex(websocketInfo.meta.address));
            } else {
                auto transaction = from_json<TransactionNotification, TransactionNotificationDto>(json);
                for (const auto& notifier : _unconfirmedAddedNotifiers[websocketInfo.meta.address]) {
                    notifier(transaction);
                }
            }
        } else if (internal::pathUnconfirmedRemoved == websocketInfo.meta.channelName) {
            if (_unconfirmedRemovedNotifiers.find(websocketInfo.meta.address) == _unconfirmedRemovedNotifiers.end()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathUnconfirmedRemoved, Address::FromHex(websocketInfo.meta.address));
            } else {
                auto transactionInfo = from_json<transactions_info::TransactionInfo, TransactionInfoDto>(json);
                for (const auto& notifier : _unconfirmedRemovedNotifiers[websocketInfo.meta.address]) {
                    notifier(transactionInfo);
                }
            }
        } else if (internal::pathStatus == websocketInfo.meta.channelName) {
            if (_statusNotifiers.find(websocketInfo.meta.address) == _statusNotifiers.end()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathStatus, Address::FromHex(websocketInfo.meta.address));
            } else {
                auto transactionStatus = from_json<TransactionStatusNotification, TransactionStatusNotificationDto>(json);
                for (const auto& notifier : _statusNotifiers[websocketInfo.meta.address]) {
                    notifier(transactionStatus);
                }
            }
        } else if (internal::pathPartialAdded == websocketInfo.meta.channelName) {
            if (_partialAddedNotifiers.find(websocketInfo.meta.address) == _partialAddedNotifiers.end()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathPartialAdded, Address::FromHex(websocketInfo.meta.address));
            } else {
                auto transaction = transaction_from_json(json);
                for (const auto& notifier : _partialAddedNotifiers[websocketInfo.meta.address]) {
                    notifier(transaction);
                }
            }
        } else if (internal::pathPartialRemoved == websocketInfo.meta.channelName) {
            if (_partialRemovedNotifiers.find(websocketInfo.meta.address) == _partialRemovedNotifiers.end()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathPartialRemoved, Address::FromHex(websocketInfo.meta.address));
            } else {
                auto transactionInfo = from_json<transactions_info::TransactionInfo, TransactionInfoDto>(json);
                for (const auto& notifier : _partialRemovedNotifiers[websocketInfo.meta.address]) {
                    notifier(transactionInfo);
                }
            }
        } else if (internal::pathCosignature == websocketInfo.meta.channelName) {
            if (_cosignatureNotifiers.find(websocketInfo.meta.address) == _cosignatureNotifiers.end()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathCosignature, Address::FromHex(websocketInfo.meta.address));
            } else {
                auto signerInfo = from_json<SignerInfoNotification, SignerInfoNotificationDto>(json);
                for (const auto& notifier : _cosignatureNotifiers[websocketInfo.meta.address]) {
                    notifier(signerInfo);
                }
            }
        } else if (internal::pathDriveState == websocketInfo.meta.channelName) {
            if (_driveStateNotifiers.find(websocketInfo.meta.address) == _driveStateNotifiers.end()) {
                _subscriptionManager->unsubscribe(_uid, internal::pathDriveState, Address::FromHex(websocketInfo.meta.address));
            } else {
                auto driveState = from_json<DriveStateNotification, DriveStateNotificationDto>(json);
                for (const auto& notifier : _driveStateNotifiers[websocketInfo.meta.address]) {
                    notifier(driveState);
                }
            }
        } else {
            std::cout << "notifications: received notification from unsupported channel: " << json << " (Ignored)" << std::endl;
        }
    }

    void NotificationService::addBlockNotifiers(const std::initializer_list<BlockNotifier> &notifiers) {
        if (empty(notifiers)) {
            XPX_CHAIN_SDK_THROW_1(notification_error, "list of notifiers is empty", __FUNCTION__)
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

    void NotificationService::addConfirmedAddedNotifiers(const Address& address, const std::initializer_list<ConfirmedAddedNotifier> &notifiers) {
        if (empty(notifiers)) {
            XPX_CHAIN_SDK_THROW_1(notification_error, "list of notifiers is empty", __FUNCTION__)
        }

        addMutexIfNotExist(internal::pathConfirmedAdded);

        auto callback = [pThis = shared_from_this(), address, notifiers]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathConfirmedAdded]);

            const std::string hexAddress = ToHex(address.binary());
            if ( pThis->_confirmedAddedNotifiers.find(hexAddress) != pThis->_confirmedAddedNotifiers.end() ) {
                pThis->_confirmedAddedNotifiers[hexAddress].insert(pThis->_confirmedAddedNotifiers[hexAddress].end(), notifiers.begin(), notifiers.end());
            } else {
                pThis->_confirmedAddedNotifiers.insert(std::pair<std::string, std::vector<ConfirmedAddedNotifier>>(hexAddress, notifiers));
                pThis->_subscriptionManager->subscribe(pThis->_uid, internal::pathConfirmedAdded, address);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::removeConfirmedAddedNotifiers(const Address& address) {
        addMutexIfNotExist(internal::pathConfirmedAdded);
        auto callback = [pThis = shared_from_this(), address]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathConfirmedAdded]);

            const std::string hexAddress = ToHex(address.binary());
            if (pThis->_confirmedAddedNotifiers.find(hexAddress) != pThis->_confirmedAddedNotifiers.end()) {
                pThis->_subscriptionManager->unsubscribe(pThis->_uid, internal::pathConfirmedAdded, address);
                pThis->_confirmedAddedNotifiers.erase(hexAddress);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::addUnconfirmedAddedNotifiers(const Address& address, const std::initializer_list<UnconfirmedAddedNotifier> &notifiers) {
        if (empty(notifiers)) {
            XPX_CHAIN_SDK_THROW_1(notification_error, "list of notifiers is empty", __FUNCTION__)
        }

        addMutexIfNotExist(internal::pathUnconfirmedAdded);

        auto callback = [pThis = shared_from_this(), address, notifiers]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathUnconfirmedAdded]);

            const std::string hexAddress = ToHex(address.binary());
            if ( pThis->_unconfirmedAddedNotifiers.find(hexAddress) != pThis->_unconfirmedAddedNotifiers.end() ) {
                pThis->_unconfirmedAddedNotifiers[hexAddress].insert(pThis->_unconfirmedAddedNotifiers[hexAddress].end(), notifiers.begin(), notifiers.end());
            } else {
                pThis->_unconfirmedAddedNotifiers.insert(std::pair<std::string, std::vector<ConfirmedAddedNotifier>>(hexAddress, notifiers));
                pThis->_subscriptionManager->subscribe(pThis->_uid, internal::pathUnconfirmedAdded, address);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::removeUnconfirmedAddedNotifiers(const Address& address) {
        addMutexIfNotExist(internal::pathUnconfirmedAdded);
        auto callback = [pThis = shared_from_this(), address]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathUnconfirmedAdded]);

            const std::string hexAddress = ToHex(address.binary());
            if (pThis->_unconfirmedAddedNotifiers.find(hexAddress) != pThis->_unconfirmedAddedNotifiers.end()) {
                pThis->_subscriptionManager->unsubscribe(pThis->_uid, internal::pathUnconfirmedAdded, address);
                pThis->_unconfirmedAddedNotifiers.erase(hexAddress);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::addUnconfirmedRemovedNotifiers(const Address& address, const std::initializer_list<UnconfirmedRemovedNotifier> &notifiers) {
        if (empty(notifiers)) {
            XPX_CHAIN_SDK_THROW_1(notification_error, "list of notifiers is empty", __FUNCTION__)
        }

        addMutexIfNotExist(internal::pathUnconfirmedRemoved);

        auto callback = [pThis = shared_from_this(), address, notifiers]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathUnconfirmedRemoved]);

            const std::string hexAddress = ToHex(address.binary());
            if ( pThis->_unconfirmedRemovedNotifiers.find(hexAddress) != pThis->_unconfirmedRemovedNotifiers.end() ) {
                pThis->_unconfirmedRemovedNotifiers[hexAddress].insert(pThis->_unconfirmedRemovedNotifiers[hexAddress].end(), notifiers.begin(), notifiers.end());
            } else {
                pThis->_unconfirmedRemovedNotifiers.insert(std::pair<std::string, std::vector<UnconfirmedRemovedNotifier>>(hexAddress, notifiers));
                pThis->_subscriptionManager->subscribe(pThis->_uid, internal::pathUnconfirmedRemoved, address);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::removeUnconfirmedRemovedNotifiers(const Address& address) {
        addMutexIfNotExist(internal::pathUnconfirmedRemoved);
        auto callback = [pThis = shared_from_this(), address]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathUnconfirmedRemoved]);

            const std::string hexAddress = ToHex(address.binary());
            if (pThis->_unconfirmedRemovedNotifiers.find(hexAddress) != pThis->_unconfirmedRemovedNotifiers.end()) {
                pThis->_subscriptionManager->unsubscribe(pThis->_uid, internal::pathUnconfirmedRemoved, address);
                pThis->_unconfirmedRemovedNotifiers.erase(hexAddress);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::addPartialAddedNotifiers(const Address& address, const std::initializer_list<PartialAddedNotifier> &notifiers) {
        if (empty(notifiers)) {
            XPX_CHAIN_SDK_THROW_1(notification_error, "list of notifiers is empty", __FUNCTION__)
        }

        addMutexIfNotExist(internal::pathPartialAdded);

        auto callback = [pThis = shared_from_this(), address, notifiers]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathPartialAdded]);

            const std::string hexAddress = ToHex(address.binary());
            if ( pThis->_partialAddedNotifiers.find(hexAddress) != pThis->_partialAddedNotifiers.end() ) {
                pThis->_partialAddedNotifiers[hexAddress].insert(pThis->_partialAddedNotifiers[hexAddress].end(), notifiers.begin(), notifiers.end());
            } else {
                pThis->_partialAddedNotifiers.insert(std::pair<std::string, std::vector<PartialAddedNotifier>>(hexAddress, notifiers));
                pThis->_subscriptionManager->subscribe(pThis->_uid, internal::pathPartialAdded, address);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::removePartialAddedNotifiers(const Address& address) {
        addMutexIfNotExist(internal::pathPartialAdded);
        auto callback = [pThis = shared_from_this(), address]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathPartialAdded]);

            const std::string hexAddress = ToHex(address.binary());
            if (pThis->_partialAddedNotifiers.find(hexAddress) != pThis->_partialAddedNotifiers.end()) {
                pThis->_subscriptionManager->unsubscribe(pThis->_uid, internal::pathPartialAdded, address);
                pThis->_partialAddedNotifiers.erase(hexAddress);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::addPartialRemovedNotifiers(const Address& address, const std::initializer_list<PartialRemovedNotifier> &notifiers) {
        if (empty(notifiers)) {
            XPX_CHAIN_SDK_THROW_1(notification_error, "list of notifiers is empty", __FUNCTION__)
        }

        addMutexIfNotExist(internal::pathPartialRemoved);

        auto callback = [pThis = shared_from_this(), address, notifiers]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathPartialRemoved]);

            const std::string hexAddress = ToHex(address.binary());
            if ( pThis->_partialRemovedNotifiers.find(hexAddress) != pThis->_partialRemovedNotifiers.end() ) {
                pThis->_partialRemovedNotifiers[hexAddress].insert(pThis->_partialRemovedNotifiers[hexAddress].end(), notifiers.begin(), notifiers.end());
            } else {
                pThis->_partialRemovedNotifiers.insert(std::pair<std::string, std::vector<PartialRemovedNotifier>>(hexAddress, notifiers));
                pThis->_subscriptionManager->subscribe(pThis->_uid, internal::pathPartialRemoved, address);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::removePartialRemovedNotifiers(const Address& address) {
        addMutexIfNotExist(internal::pathPartialRemoved);
        auto callback = [pThis = shared_from_this(), address]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathPartialRemoved]);

            const std::string hexAddress = ToHex(address.binary());
            if (pThis->_partialRemovedNotifiers.find(hexAddress) != pThis->_partialRemovedNotifiers.end()) {
                pThis->_subscriptionManager->unsubscribe(pThis->_uid, internal::pathPartialRemoved, address);
                pThis->_partialRemovedNotifiers.erase(hexAddress);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::addStatusNotifiers(const Address& address, const std::initializer_list<StatusNotifier>& notifiers) {
        if (empty(notifiers)) {
            XPX_CHAIN_SDK_THROW_1(notification_error, "list of notifiers is empty", __FUNCTION__)
        }

        addMutexIfNotExist(internal::pathStatus);

        auto callback = [pThis = shared_from_this(), address, notifiers]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathStatus]);

            const std::string hexAddress = ToHex(address.binary());
            if ( pThis->_statusNotifiers.find(hexAddress) != pThis->_statusNotifiers.end() ) {
                pThis->_statusNotifiers[hexAddress].insert(pThis->_statusNotifiers[hexAddress].end(), notifiers.begin(), notifiers.end());
            } else {
                pThis->_statusNotifiers.insert(std::pair<std::string, std::vector<StatusNotifier>>(hexAddress, notifiers));
                pThis->_subscriptionManager->subscribe(pThis->_uid, internal::pathStatus, address);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::removeStatusNotifiers(const Address& address) {
        addMutexIfNotExist(internal::pathStatus);
        auto callback = [pThis = shared_from_this(), address]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathStatus]);

            const std::string hexAddress = ToHex(address.binary());
            if (pThis->_statusNotifiers.find(hexAddress) != pThis->_statusNotifiers.end()) {
                pThis->_subscriptionManager->unsubscribe(pThis->_uid, internal::pathStatus, address);
                pThis->_statusNotifiers.erase(hexAddress);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::addCosignatureNotifiers(const Address& address, const std::initializer_list<CosignatureNotifier> &notifiers) {
        if (empty(notifiers)) {
            XPX_CHAIN_SDK_THROW_1(notification_error, "list of notifiers is empty", __FUNCTION__)
        }

        addMutexIfNotExist(internal::pathCosignature);

        auto callback = [pThis = shared_from_this(), address, notifiers]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathCosignature]);

            const std::string hexAddress = ToHex(address.binary());
            if ( pThis->_cosignatureNotifiers.find(hexAddress) != pThis->_cosignatureNotifiers.end() ) {
                pThis->_cosignatureNotifiers[hexAddress].insert(pThis->_cosignatureNotifiers[hexAddress].end(), notifiers.begin(), notifiers.end());
            } else {
                pThis->_cosignatureNotifiers.insert(std::pair<std::string, std::vector<CosignatureNotifier>>(hexAddress, notifiers));
                pThis->_subscriptionManager->subscribe(pThis->_uid, internal::pathCosignature, address);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::removeCosignatureNotifiers(const Address& address) {
        addMutexIfNotExist(internal::pathCosignature);
        auto callback = [pThis = shared_from_this(), address]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathCosignature]);

            const std::string hexAddress = ToHex(address.binary());
            if (pThis->_cosignatureNotifiers.find(hexAddress) != pThis->_cosignatureNotifiers.end()) {
                pThis->_subscriptionManager->unsubscribe(pThis->_uid, internal::pathCosignature, address);
                pThis->_cosignatureNotifiers.erase(hexAddress);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::addDriveStateNotifiers(const Address& address, const std::initializer_list<DriveStateNotifier> &notifiers) {
        if (empty(notifiers)) {
            XPX_CHAIN_SDK_THROW_1(notification_error, "list of notifiers is empty", __FUNCTION__)
        }

        addMutexIfNotExist(internal::pathDriveState);

        auto callback = [pThis = shared_from_this(), address, notifiers]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathDriveState]);

            const std::string hexAddress = ToHex(address.binary());
            if ( pThis->_driveStateNotifiers.find(hexAddress) != pThis->_driveStateNotifiers.end() ) {
                pThis->_driveStateNotifiers[hexAddress].insert(pThis->_driveStateNotifiers[hexAddress].end(), notifiers.begin(), notifiers.end());
            } else {
                pThis->_driveStateNotifiers.insert(std::pair<std::string, std::vector<DriveStateNotifier>>(hexAddress, notifiers));
                pThis->_subscriptionManager->subscribe(pThis->_uid, internal::pathDriveState, address);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }

    void NotificationService::removeDriveStateNotifiers(const Address& address) {
        addMutexIfNotExist(internal::pathDriveState);
        auto callback = [pThis = shared_from_this(), address]() {
            std::lock_guard<std::mutex> lock(*pThis->_notifiersMutexes[internal::pathDriveState]);

            const std::string hexAddress = ToHex(address.binary());
            if (pThis->_driveStateNotifiers.find(hexAddress) != pThis->_driveStateNotifiers.end()) {
                pThis->_subscriptionManager->unsubscribe(pThis->_uid, internal::pathDriveState, address);
                pThis->_driveStateNotifiers.erase(hexAddress);
            }
        };

        if (!_isInitialized) {
            initialize(callback);
        } else {
            callback();
        }
    }
}