/**
*** Copyright 2021 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <string>
#include <xpxchaincpp/model/transaction_simple/transaction.h>

namespace xpx_chain_sdk {

    class TransactionData {
    public:
        std::string signature;
        std::string signer;
        int32_t version;
        TransactionType type;
        int64_t maxFee;
        int64_t deadline;
    };

    class TransactionMeta {
    public:
        std::string hash;
        std::string merkleComponentHash;
        uint64_t height;
    };

    class TransactionNotification {
    public:
        TransactionData data;
        TransactionMeta meta;
    };
}
