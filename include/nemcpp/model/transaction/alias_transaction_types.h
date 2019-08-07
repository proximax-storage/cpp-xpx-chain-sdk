
#pragma once

#include <cstdint>

namespace xpx_sdk::difficult_transactions {
	
	/// Alias transaction action.
	enum class AliasTransactionAction: uint8_t {
		Link,
		Unlink
	};
}
