/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/types.h>

#include <string>
#include <optional>

namespace xpx_chain_sdk {

	struct AutomaticExecutionsInfo {
		std::string AutomaticExecutionFileName;
		std::string AutomaticExecutionsFunctionName;
		Height AutomaticExecutionsNextBlockToCheck;
		Amount AutomaticExecutionCallPayment;
		Amount AutomaticDownloadCallPayment;
		uint32_t AutomatedExecutionsNumber = 0U;
		std::optional<Height> AutomaticExecutionsPrepaidSince;
	};

	/// Returns \c true if \a lhs and \a rhs are equal.
	bool operator==(const AutomaticExecutionsInfo& lhs, const AutomaticExecutionsInfo& rhs);

	/// Returns \c true if \a lhs and \a rhs are not equal.
	bool operator!=(const AutomaticExecutionsInfo& lhs, const AutomaticExecutionsInfo& rhs);
}