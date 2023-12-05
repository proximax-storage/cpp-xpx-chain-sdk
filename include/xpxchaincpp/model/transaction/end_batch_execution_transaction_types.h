/**
*** Copyright 2023 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/model/transaction/transaction.h>
#include <xpxchaincpp/model/transaction/embedded_transaction.h>
#include <xpxchaincpp/model/transaction/end_batch_execution_transaction_types.h>

#include <optional>

namespace xpx_chain_sdk {

	class ExtendedCallDigest {
	public:
		Hash256 CallId;
		bool Manual;
		uint64_t Block;
		int16_t Status;
		Hash256 ReleasedTransactionHash;
	};

	class RawProofOfExecution {
	public:
		uint64_t StartBatchId;
		std::array<uint8_t, 32> T;
		std::array<uint8_t, 32> R;
		std::array<uint8_t, 32> F;
		std::array<uint8_t, 32> K;
	};

	class CallPayment {
	public:
		Amount ExecutionPayment;
		Amount DownloadPayment;
	};

	struct Opinion {
		Hash256 publicKey;
		Hash256 signature;
		std::vector<RawProofOfExecution> poEx;
		std::vector<CallPayment> callPayments;
	};
}
