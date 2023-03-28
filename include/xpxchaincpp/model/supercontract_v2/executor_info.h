/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/types.h>

#include <string>
#include <string_view>

namespace xpx_chain_sdk {

	struct ProofOfExecution {
		uint64_t StartBatchId = 0;
		crypto::CurvePoint T;
		crypto::Scalar R;
	};

    /// Returns \c true if \a lhs and \a rhs are equal.
    bool operator==(const ProofOfExecution& lhs, const ProofOfExecution& rhs);

    /// Returns \c true if \a lhs and \a rhs are not equal.
    bool operator!=(const ProofOfExecution& lhs, const ProofOfExecution& rhs);

	struct ExecutorInfo {
		uint64_t NextBatchToApprove = 0;
		ProofOfExecution PoEx;
	};

    /// Returns \c true if \a lhs and \a rhs are equal.
    bool operator==(const ExecutorInfo& lhs, const ExecutorInfo& rhs);

    /// Returns \c true if \a lhs and \a rhs are not equal.
    bool operator!=(const ExecutorInfo& lhs, const ExecutorInfo& rhs);

}