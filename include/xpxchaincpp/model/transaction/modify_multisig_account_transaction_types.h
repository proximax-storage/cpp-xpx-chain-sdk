/**
*** Copyright 2019 ProximaX Limited. All rights reserved.
*** Use of this source code is governed by the Apache 2.0
*** license that can be found in the LICENSE file.
**/
#pragma once

#include <xpxchaincpp/model/account/public_account.h>

#include <unordered_map>

namespace xpx_chain_sdk {
	
	/// Multisig account cosignatory modification type.
	enum class CosignatoryModificationType: uint8_t {
		Add,
		Remove
	};
	
	/// Cosignatory modifications container.
	using CosignatoryModifications = std::unordered_map<PublicAccount, CosignatoryModificationType>;
}
