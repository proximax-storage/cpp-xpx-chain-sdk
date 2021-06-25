/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <memory>
#include <xpxchaincpp/model/account/address.h>
#include "../network/websocket.h"

namespace xpx_chain_sdk::internal {

    using namespace xpx_chain_sdk::internal::network;

    const std::string pathBlock = "block";
    const std::string pathConfirmedAdded = "confirmedAdded";
    const std::string pathUnconfirmedAdded = "unconfirmedAdded";
    const std::string pathUnconfirmedRemoved = "unconfirmedRemoved";
    const std::string pathStatus = "status";
    const std::string pathPartialAdded = "partialAdded";
    const std::string pathPartialRemoved = "partialRemoved";
    const std::string pathCosignature = "cosignature";
    const std::string pathDriveState = "driveState";

    class SubscriptionManager {
    public:
        explicit SubscriptionManager(std::shared_ptr<WsClient> wsClient);
        ~SubscriptionManager() = default;

    public:
        void subscribe(const std::string& uid, const std::string& path);
        void subscribe(const std::string& uid, const std::string& path, const Address& address);
        void unsubscribe(const std::string& uid, const std::string& path);
        void unsubscribe(const std::string& uid, const std::string& path, const Address& address);

    private:
        static std::string createJson(const std::string &uid, const std::string &path, const std::string& type) ;

    private:
        std::shared_ptr<WsClient> _wsClient;
    };
}