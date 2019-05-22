
#pragma once

#include <nemcpp/model/account/public_account.h>

#include <unordered_map>

namespace nem2_sdk {
	
	/// Multisig account cosignatory modification type.
	enum class CosignatoryModificationType: uint8_t {
		Add,
		Remove
	};
	
	/// Cosignatory modifications container.
	using CosignatoryModifications = std::unordered_map<PublicAccount, CosignatoryModificationType>;
}
