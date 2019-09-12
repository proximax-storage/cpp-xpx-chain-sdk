
#pragma once

#include <cstdint>

namespace xpx_chain_sdk {
	
	/// Alias transaction action.
	enum class AliasTransactionAction: uint8_t {
		Link,
		Unlink
	};
}
