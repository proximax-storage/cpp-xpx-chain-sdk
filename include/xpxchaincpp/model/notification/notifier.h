/**
*** Copyright 2022 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <string>
#include <utility>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <xpxchaincpp/model/blockchain/block.h>
#include <xpxchaincpp/model/transaction_simple/transaction.h>
#include <xpxchaincpp/model/transaction_simple/transaction_info.h>
#include <xpxchaincpp/model/notification/drive_state_notification.h>
#include <xpxchaincpp/model/notification/transaction_notification.h>
#include <xpxchaincpp/model/notification/transaction_status_notification.h>
#include <xpxchaincpp/model/notification/signer_info_notification.h>

namespace xpx_chain_sdk {

    template <typename NotificationType>
    class Notifier {
    public:
        Notifier() = delete;

        explicit Notifier(std::function<void(const std::string&, const NotificationType&)> n) :
        uuid(generateUuid()),
        notifier(n) {}

        ~Notifier() = default;

    public:
        std::string getId() const {
            return uuid;
        }

        void run(const NotificationType& notification) const {
            notifier(uuid, notification);
        }

    private:
        static std::string generateUuid() {
            boost::uuids::uuid id = boost::uuids::random_generator()();
            return boost::uuids::to_string(id);
        }

    private:
        std::string uuid;
        std::function<void(const std::string& notifierId, const NotificationType&)> notifier;
    };
}
