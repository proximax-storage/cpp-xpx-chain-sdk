
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
