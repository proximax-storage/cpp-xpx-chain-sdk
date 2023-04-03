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
		std::string automaticExecutionFileName;
		std::string automaticExecutionsFunctionName;
		uint64_t automaticExecutionsNextBlockToCheck;
		uint64_t automaticExecutionCallPayment;
		uint64_t automaticDownloadCallPayment;
		uint32_t automatedExecutionsNumber = 0U;
		std::optional<uint64_t> automaticExecutionsPrepaidSince;
	};
}