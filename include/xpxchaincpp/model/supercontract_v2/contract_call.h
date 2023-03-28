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

	struct ServicePayment {
		MosaicId MosaicId;
		Amount Amount;
	};

	/// Returns \c true if \a lhs and \a rhs are equal.
	bool operator==(const ServicePayment& lhs, const ServicePayment& rhs);

	/// Returns \c true if \a lhs and \a rhs are not equal.
	bool operator!=(const ServicePayment& lhs, const ServicePayment& rhs);

	struct ContractCall {
		Hash256 CallId;
		Key Caller;
		std::string FileName;
		std::string FunctionName;
		std::string ActualArguments;
		Amount ExecutionCallPayment;
		Amount DownloadCallPayment;
		std::vector<ServicePayment> ServicePayments;
		Height BlockHeight;
	};

	/// Returns \c true if \a lhs and \a rhs are equal.
	bool operator==(const ContractCall& lhs, const ContractCall& rhs);

	/// Returns \c true if \a lhs and \a rhs are not equal.
	bool operator!=(const ContractCall& lhs, const ContractCall& rhs);
}