
#pragma once

#include <cstdint>

namespace nem2_sdk {
	
	/// Alias transaction action.
	enum class AliasTransactionAction: uint8_t {
		Link,
		Unlink
	};
}
