/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/types.h>
#include <xpxchaincpp/utils/comparator.h>
#include <xpxchaincpp/model/transaction/transaction_type.h>

#include <cstdint>
#include <set>

namespace xpx_chain_sdk {

    struct ExtendedCallDigest {
        Hash256 CallId;
        bool Manual;
        Height Block;
        int16_t Status;
        Hash256 ReleasedTransactionHash;
    };

    struct RawProofOfExecution {
        uint64_t StartBatchId;
        std::array<uint8_t, 32> T;
        std::array<uint8_t, 32> R;
        std::array<uint8_t, 32> F;
        std::array<uint8_t, 32> K;
    };

    struct CallPayment {
        Amount ExecutionPayment;
        Amount DownloadPayment;
    };

    struct Opinion {
        Key publicKey;
        Hash256 signature;
        std::vector<RawProofOfExecution> poEx;
        std::vector<CallPayment> callPayments;
    };
}
