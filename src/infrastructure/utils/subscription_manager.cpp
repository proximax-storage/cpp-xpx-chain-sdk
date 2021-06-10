/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/

#include "subscription_manager.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>

namespace xpx_chain_sdk::internal {

    SubscriptionManager::SubscriptionManager(std::shared_ptr<WsClient> wsClient)
        : _wsClient(wsClient) {}

    void SubscriptionManager::subscribe(const std::string &uid, const std::string &path) {
        _wsClient->send(createJson(uid, path, "subscribe"));
    }

    void SubscriptionManager::subscribe(const std::string &uid, const std::string &path, const Address& address) {
        _wsClient->send(createJson(uid, path + "/" + address.encoded(), "subscribe"));
    }

    void SubscriptionManager::unsubscribe(const std::string &uid, const std::string &path) {
        _wsClient->send(createJson(uid, path, "unsubscribe"));
    }

    void SubscriptionManager::unsubscribe(const std::string &uid, const std::string &path, const Address &address) {
        _wsClient->send(createJson(uid, path + "/" + address.encoded(), "unsubscribe"));
    }

    std::string SubscriptionManager::createJson(const std::string &uid, const std::string &path, const std::string& type) {
        boost::property_tree::ptree ptree;
        ptree.put("uid", uid);
        ptree.put(type, path);

        std::ostringstream oss;
        boost::property_tree::write_json(oss, ptree);

        return oss.str();
    }
}
