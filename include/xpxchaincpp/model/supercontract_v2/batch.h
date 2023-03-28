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

	struct CompletedCall {
		Hash256 CallId;
		// Zero means that it's an automatic execution
		Key Caller;
		// Zero Status Means Success
		int16_t Status = 0;
		Amount ExecutionWork;
		Amount DownloadWork;
	};

	/// Returns \c true if \a lhs and \a rhs are equal.
	bool operator==(const CompletedCall& lhs, const CompletedCall& rhs);

	/// Returns \c true if \a lhs and \a rhs are not equal.
	bool operator!=(const CompletedCall& lhs, const CompletedCall& rhs);

	struct Batch{
		bool Success = false;
//		crypto::CurvePoint PoExVerificationInformation;
		std::vector<CompletedCall> CompletedCalls;
	};

    /// Returns \c true if \a lhs and \a rhs are equal.
    bool operator==(const Batch& lhs, const Batch& rhs);

    /// Returns \c true if \a lhs and \a rhs are not equal.
    bool operator!=(const Batch& lhs, const Batch& rhs);
}